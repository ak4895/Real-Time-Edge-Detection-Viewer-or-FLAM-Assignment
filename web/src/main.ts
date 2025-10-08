// Real-Time Edge Detection Viewer
// Supports: Static images, file upload, and live webcam

class EdgeViewer {
    private canvas: HTMLCanvasElement;
    private ctx: CanvasRenderingContext2D;
    private image: HTMLImageElement | null = null;
    private video: HTMLVideoElement | null = null;
    private stream: MediaStream | null = null;
    private webcamActive = false;
    private tempCanvas: HTMLCanvasElement;
    private tempCtx: CanvasRenderingContext2D;
    private frameCount = 0;
    private lastFpsTime = 0;

    constructor() {
        this.canvas = document.getElementById('viewerCanvas') as HTMLCanvasElement;
        this.ctx = this.canvas.getContext('2d')!;
        this.tempCanvas = document.createElement('canvas');
        this.tempCtx = this.tempCanvas.getContext('2d')!;
        
        this.setupEventListeners();
        this.drawWelcome();
        this.startAnimationLoop();
        
        console.log('Edge Detection Viewer initialized');
    }

    private setupEventListeners(): void {
        const loadBtn = document.getElementById('loadSampleBtn');
        const captureBtn = document.getElementById('captureBtn');
        const uploadInput = document.getElementById('imageUpload') as HTMLInputElement;

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

    private loadSample(): void {
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

    private handleFileUpload(event: Event): void {
        this.stopWebcam();
        
        const input = event.target as HTMLInputElement;
        const file = input.files?.[0];
        
        if (!file) return;
        
        console.log('Processing uploaded file:', file.name);
        
        const reader = new FileReader();
        
        reader.onload = (e) => {
            const img = new Image();
            
            img.onload = () => {
                console.log('Image loaded:', img.width, 'x', img.height);
                
                // Resize canvases to match image
                this.canvas.width = img.width;
                this.canvas.height = img.height;
                this.tempCanvas.width = img.width;
                this.tempCanvas.height = img.height;
                
                // Apply edge detection to uploaded image
                this.applyEdgeDetection(img);
                this.updateResolution();
                
                console.log('Edge detection applied to uploaded image');
            };
            
            img.src = e.target?.result as string;
        };
        
        reader.readAsDataURL(file);
    }

    private async toggleWebcam(): Promise<void> {
        if (this.webcamActive) {
            this.stopWebcam();
        } else {
            await this.startWebcam();
        }
    }

    private async startWebcam(): Promise<void> {
        console.log('Starting webcam...');
        
        try {
            this.stream = await navigator.mediaDevices.getUserMedia({
                video: {
                    width: { ideal: 1280 },
                    height: { ideal: 720 }
                }
            });
            
            this.video = document.createElement('video');
            this.video.srcObject = this.stream;
            this.video.autoplay = true;
            this.video.playsInline = true;
            
            await this.video.play();
            
            this.video.onloadedmetadata = () => {
                if (!this.video) return;
                
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
                
                // Update button text
                const btn = document.getElementById('captureBtn');
                if (btn) btn.textContent = '⏹️ Stop Webcam';
            };
            
        } catch (error) {
            console.error('Webcam error:', error);
            alert('Could not access webcam. Please grant camera permission.');
        }
    }

    private stopWebcam(): void {
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
        if (btn) btn.textContent = '📷 Use Webcam';
    }

    private applyEdgeDetection(source: HTMLImageElement | HTMLVideoElement): void {
        // Draw source to temp canvas
        this.tempCtx.drawImage(source, 0, 0, this.tempCanvas.width, this.tempCanvas.height);
        
        // Get image data
        const imageData = this.tempCtx.getImageData(0, 0, this.tempCanvas.width, this.tempCanvas.height);
        const data = imageData.data;
        const width = imageData.width;
        const height = imageData.height;
        const output = new Uint8ClampedArray(data.length);
        
        // Apply Sobel edge detection
        for (let y = 1; y < height - 1; y++) {
            for (let x = 1; x < width - 1; x++) {
                let gx = 0;
                let gy = 0;
                
                // Sobel operator
                for (let dy = -1; dy <= 1; dy++) {
                    for (let dx = -1; dx <= 1; dx++) {
                        const idx = ((y + dy) * width + (x + dx)) * 4;
                        const gray = (data[idx] + data[idx + 1] + data[idx + 2]) / 3;
                        
                        // Sobel kernels approximation
                        gx += gray * dx * (Math.abs(dy) + 1);
                        gy += gray * dy * (Math.abs(dx) + 1);
                    }
                }
                
                // Calculate gradient magnitude
                const magnitude = Math.sqrt(gx * gx + gy * gy);
                const edge = magnitude > 50 ? 255 : 0;
                
                const idx = (y * width + x) * 4;
                output[idx] = edge;
                output[idx + 1] = edge;
                output[idx + 2] = edge;
                output[idx + 3] = 255;
            }
        }
        
        // Draw edges to main canvas
        const outputImageData = new ImageData(output, width, height);
        this.ctx.putImageData(outputImageData, 0, 0);
    }

    private drawWelcome(): void {
        this.ctx.fillStyle = '#000000';
        this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);
        
        // Draw grid pattern
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
        
        // Draw welcome text
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

    private startAnimationLoop(): void {
        this.lastFpsTime = performance.now();
        
        const animate = (currentTime: number) => {
            this.frameCount++;
            
            // Update FPS every second
            if (currentTime - this.lastFpsTime >= 1000) {
                const fps = Math.round(this.frameCount * 1000 / (currentTime - this.lastFpsTime));
                this.updateFPS(fps);
                this.frameCount = 0;
                this.lastFpsTime = currentTime;
            }
            
            // Render current frame
            this.render();
            
            requestAnimationFrame(animate);
        };
        
        requestAnimationFrame(animate);
    }

    private render(): void {
        if (this.webcamActive && this.video) {
            // Live webcam with real-time edge detection
            this.applyEdgeDetection(this.video);
        } else if (this.image && this.image.complete) {
            // Static image - just display it
            this.ctx.drawImage(this.image, 0, 0, this.canvas.width, this.canvas.height);
        }
    }

    private updateFPS(fps: number): void {
        const fpsElement = document.getElementById('fps');
        if (fpsElement) {
            fpsElement.textContent = fps.toString();
        }
    }

    private updateResolution(): void {
        const resElement = document.getElementById('res');
        if (resElement) {
            resElement.textContent = `${this.canvas.width} x ${this.canvas.height}`;
        }
    }
}

// Initialize when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    new EdgeViewer();
    console.log('🚀 Edge Detection Viewer ready!');
});

export {};
