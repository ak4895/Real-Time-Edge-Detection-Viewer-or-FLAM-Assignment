"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
class EdgeViewer {
    constructor() {
        this.image = null;
        this.video = null;
        this.stream = null;
        this.webcamActive = false;
        this.frameCount = 0;
        this.lastFpsTime = 0;
        this.canvas = document.getElementById('viewerCanvas');
        this.ctx = this.canvas.getContext('2d');
        this.tempCanvas = document.createElement('canvas');
        this.tempCtx = this.tempCanvas.getContext('2d');
        this.setupEventListeners();
        this.drawWelcome();
        this.startAnimationLoop();
        console.log('Edge Detection Viewer initialized');
    }
    setupEventListeners() {
        const loadBtn = document.getElementById('loadSampleBtn');
        const captureBtn = document.getElementById('captureBtn');
        const uploadInput = document.getElementById('imageUpload');
        console.log('Setting up event listeners...', { loadBtn, captureBtn, uploadInput });
        if (loadBtn) {
            loadBtn.addEventListener('click', () => {
                console.log('Load Sample button clicked');
                this.loadSample();
            });
        }
        if (captureBtn) {
            captureBtn.addEventListener('click', () => {
                console.log('Webcam button clicked');
                this.toggleWebcam();
            });
        }
        if (uploadInput) {
            uploadInput.addEventListener('change', (e) => {
                console.log('File upload input changed');
                this.handleFileUpload(e);
            });
        }
    }
    loadSample() {
        console.log('Loading sample image...');
        this.stopWebcam();
        const img = new Image();
        img.onload = () => {
            console.log('Sample loaded:', img.width, 'x', img.height);
            this.image = img;
            this.canvas.width = img.width;
            this.canvas.height = img.height;
            this.updateResolution();
        };
        img.onerror = () => {
            console.error('Failed to load sample image');
            alert('Sample image not found at assets/sample-edge-frame.png');
        };
        img.src = 'assets/sample-edge-frame.png';
    }
    handleFileUpload(event) {
        var _a;
        this.stopWebcam();
        const input = event.target;
        const file = (_a = input.files) === null || _a === void 0 ? void 0 : _a[0];
        if (!file)
            return;
        console.log('Processing uploaded file:', file.name);
        const reader = new FileReader();
        reader.onload = (e) => {
            var _a;
            const img = new Image();
            img.onload = () => {
                console.log('Image loaded:', img.width, 'x', img.height);
                this.canvas.width = img.width;
                this.canvas.height = img.height;
                this.tempCanvas.width = img.width;
                this.tempCanvas.height = img.height;
                this.applyEdgeDetection(img);
                this.updateResolution();
                console.log('Edge detection applied to uploaded image');
            };
            img.src = (_a = e.target) === null || _a === void 0 ? void 0 : _a.result;
        };
        reader.readAsDataURL(file);
    }
    toggleWebcam() {
        return __awaiter(this, void 0, void 0, function* () {
            if (this.webcamActive) {
                this.stopWebcam();
            }
            else {
                yield this.startWebcam();
            }
        });
    }
    startWebcam() {
        return __awaiter(this, void 0, void 0, function* () {
            console.log('Starting webcam...');
            try {
                this.stream = yield navigator.mediaDevices.getUserMedia({
                    video: {
                        width: { ideal: 1280 },
                        height: { ideal: 720 }
                    }
                });
                this.video = document.createElement('video');
                this.video.srcObject = this.stream;
                this.video.autoplay = true;
                this.video.playsInline = true;
                yield this.video.play();
                this.video.onloadedmetadata = () => {
                    if (!this.video)
                        return;
                    const width = this.video.videoWidth;
                    const height = this.video.videoHeight;
                    console.log('Webcam started:', width, 'x', height);
                    this.canvas.width = width;
                    this.canvas.height = height;
                    this.tempCanvas.width = width;
                    this.tempCanvas.height = height;
                    this.webcamActive = true;
                    this.image = null;
                    this.updateResolution();
                    const btn = document.getElementById('captureBtn');
                    if (btn)
                        btn.textContent = '⏹️ Stop Webcam';
                };
            }
            catch (error) {
                console.error('Webcam error:', error);
                alert('Could not access webcam. Please grant camera permission.');
            }
        });
    }
    stopWebcam() {
        if (this.stream) {
            console.log('Stopping webcam...');
            this.stream.getTracks().forEach(track => track.stop());
            this.stream = null;
        }
        if (this.video) {
            this.video.srcObject = null;
            this.video = null;
        }
        this.webcamActive = false;
        const btn = document.getElementById('captureBtn');
        if (btn)
            btn.textContent = '📷 Use Webcam';
    }
    applyEdgeDetection(source) {
        this.tempCtx.drawImage(source, 0, 0, this.tempCanvas.width, this.tempCanvas.height);
        const imageData = this.tempCtx.getImageData(0, 0, this.tempCanvas.width, this.tempCanvas.height);
        const data = imageData.data;
        const width = imageData.width;
        const height = imageData.height;
        const output = new Uint8ClampedArray(data.length);
        for (let y = 1; y < height - 1; y++) {
            for (let x = 1; x < width - 1; x++) {
                let gx = 0;
                let gy = 0;
                for (let dy = -1; dy <= 1; dy++) {
                    for (let dx = -1; dx <= 1; dx++) {
                        const idx = ((y + dy) * width + (x + dx)) * 4;
                        const gray = (data[idx] + data[idx + 1] + data[idx + 2]) / 3;
                        gx += gray * dx * (Math.abs(dy) + 1);
                        gy += gray * dy * (Math.abs(dx) + 1);
                    }
                }
                const magnitude = Math.sqrt(gx * gx + gy * gy);
                const edge = magnitude > 50 ? 255 : 0;
                const idx = (y * width + x) * 4;
                output[idx] = edge;
                output[idx + 1] = edge;
                output[idx + 2] = edge;
                output[idx + 3] = 255;
            }
        }
        const outputImageData = new ImageData(output, width, height);
        this.ctx.putImageData(outputImageData, 0, 0);
    }
    drawWelcome() {
        this.ctx.fillStyle = '#000000';
        this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);
        this.ctx.strokeStyle = '#333333';
        this.ctx.lineWidth = 1;
        for (let x = 0; x < this.canvas.width; x += 40) {
            this.ctx.beginPath();
            this.ctx.moveTo(x, 0);
            this.ctx.lineTo(x, this.canvas.height);
            this.ctx.stroke();
        }
        for (let y = 0; y < this.canvas.height; y += 40) {
            this.ctx.beginPath();
            this.ctx.moveTo(0, y);
            this.ctx.lineTo(this.canvas.width, y);
            this.ctx.stroke();
        }
        this.ctx.fillStyle = '#00d4ff';
        this.ctx.font = 'bold 28px Arial';
        this.ctx.textAlign = 'center';
        this.ctx.fillText('Edge Detection Viewer', this.canvas.width / 2, this.canvas.height / 2 - 40);
        this.ctx.fillStyle = '#ffffff';
        this.ctx.font = '18px Arial';
        this.ctx.fillText('Click buttons above to get started', this.canvas.width / 2, this.canvas.height / 2 + 10);
        this.ctx.font = '14px Arial';
        this.ctx.fillStyle = '#888888';
        this.ctx.fillText('Load Sample • Upload Image • Use Webcam', this.canvas.width / 2, this.canvas.height / 2 + 40);
    }
    startAnimationLoop() {
        this.lastFpsTime = performance.now();
        const animate = (currentTime) => {
            this.frameCount++;
            if (currentTime - this.lastFpsTime >= 1000) {
                const fps = Math.round(this.frameCount * 1000 / (currentTime - this.lastFpsTime));
                this.updateFPS(fps);
                this.frameCount = 0;
                this.lastFpsTime = currentTime;
            }
            this.render();
            requestAnimationFrame(animate);
        };
        requestAnimationFrame(animate);
    }
    render() {
        if (this.webcamActive && this.video) {
            this.applyEdgeDetection(this.video);
        }
        else if (this.image && this.image.complete) {
            this.ctx.drawImage(this.image, 0, 0, this.canvas.width, this.canvas.height);
        }
    }
    updateFPS(fps) {
        const fpsElement = document.getElementById('fps');
        if (fpsElement) {
            fpsElement.textContent = fps.toString();
        }
    }
    updateResolution() {
        const resElement = document.getElementById('res');
        if (resElement) {
            resElement.textContent = `${this.canvas.width} x ${this.canvas.height}`;
        }
    }
}
document.addEventListener('DOMContentLoaded', () => {
    new EdgeViewer();
    console.log('🚀 Edge Detection Viewer ready!');
});
