package com.example.edgedetection

import android.Manifest
import android.content.pm.PackageManager
import android.graphics.ImageFormat
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.camera.core.*
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.example.edgedetection.databinding.ActivityMainBinding
import java.nio.ByteBuffer
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors
import kotlin.math.roundToInt

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding
    private lateinit var cameraExecutor: ExecutorService
    private lateinit var glRenderer: GLRenderer

    // Native wrapper for JNI calls
    private lateinit var nativeWrapper: NativeWrapper

    // Performance monitoring
    private var frameCount = 0
    private var lastFpsUpdateTime = System.currentTimeMillis()
    private var fps = 0

    companion object {
        private const val TAG = "MainActivity"
        private const val REQUEST_CODE_CAMERA = 10
        private const val FPS_UPDATE_INTERVAL = 1000 // 1 second
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Initialize native components
        nativeWrapper = NativeWrapper()
        nativeWrapper.initNative()

        // Initialize OpenGL renderer
        glRenderer = GLRenderer()
        binding.glSurfaceView.setEGLContextClientVersion(2)
        binding.glSurfaceView.setRenderer(glRenderer)

        // Check camera permission
        if (allPermissionsGranted()) {
            startCamera()
        } else {
            ActivityCompat.requestPermissions(
                this, arrayOf(Manifest.permission.CAMERA),
                REQUEST_CODE_CAMERA
            )
        }

        // Initialize camera executor
        cameraExecutor = Executors.newSingleThreadExecutor()
    }

    private fun allPermissionsGranted() = ContextCompat.checkSelfPermission(
        baseContext, Manifest.permission.CAMERA
    ) == PackageManager.PERMISSION_GRANTED

    override fun onRequestPermissionsResult(
        requestCode: Int, permissions: Array<String>, grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (requestCode == REQUEST_CODE_CAMERA) {
            if (allPermissionsGranted()) {
                startCamera()
            } else {
                Toast.makeText(
                    this,
                    getString(R.string.permission_denied),
                    Toast.LENGTH_SHORT
                ).show()
                finish()
            }
        }
    }

    private fun startCamera() {
        val cameraProviderFuture = ProcessCameraProvider.getInstance(this)

        cameraProviderFuture.addListener({
            // Used to bind the lifecycle of cameras to the lifecycle owner
            val cameraProvider: ProcessCameraProvider = cameraProviderFuture.get()

            // Image analysis configuration
            val imageAnalyzer = ImageAnalysis.Builder()
                .setTargetAspectRatio(AspectRatio.RATIO_16_9)
                .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
                .build()
                .also { imageAnalysis ->
                    imageAnalysis.setAnalyzer(cameraExecutor) { imageProxy ->
                        processImage(imageProxy)
                    }
                }

            // Select back camera as a default
            val cameraSelector = CameraSelector.DEFAULT_BACK_CAMERA

            try {
                // Unbind use cases before rebinding
                cameraProvider.unbindAll()

                // Bind use cases to camera
                cameraProvider.bindToLifecycle(
                    this, cameraSelector, imageAnalyzer
                )

                Log.d(TAG, "Camera initialized successfully")
            } catch (exc: Exception) {
                Log.e(TAG, "Use case binding failed", exc)
            }
        }, ContextCompat.getMainExecutor(this))
    }

    private fun processImage(imageProxy: ImageProxy) {
        if (imageProxy.format != ImageFormat.YUV_420_888) {
            Log.e(TAG, "Unsupported image format: ${imageProxy.format}")
            imageProxy.close()
            return
        }

        val data = imageProxy.toNv21ByteArray()
        val width = imageProxy.width
        val height = imageProxy.height
        val rotation = imageProxy.imageInfo.rotationDegrees

        imageProxy.close()

        // Queue the native processing and texture update on the GL thread.
        binding.glSurfaceView.queueEvent {
            val textureId = nativeWrapper.processFrame(data, width, height, rotation)
            glRenderer.updateTextureId(textureId)
        }

        // Update FPS counter
        frameCount++
        val currentTime = System.currentTimeMillis()
        if (currentTime - lastFpsUpdateTime >= FPS_UPDATE_INTERVAL) {
            fps = (frameCount * 1000.0 / (currentTime - lastFpsUpdateTime)).roundToInt()
            frameCount = 0
            lastFpsUpdateTime = currentTime

            // Update UI with frame stats
            runOnUiThread {
                binding.fpsTextView.text = getString(R.string.fps_text, fps)
                binding.resolutionTextView.text = getString(R.string.resolution_text, width, height)
            }
        }
    }

    private fun ImageProxy.toNv21ByteArray(): ByteArray {
        val yBuffer = planes[0].buffer
        val uBuffer = planes[1].buffer
        val vBuffer = planes[2].buffer

        val ySize = yBuffer.remaining()
        val uSize = uBuffer.remaining()
        val vSize = vBuffer.remaining()

        val nv21 = ByteArray(ySize + uSize + vSize)

        yBuffer.get(nv21, 0, ySize)
        vBuffer.get(nv21, ySize, vSize)

        val lastUByte = uBuffer.get(uSize - 1)
        nv21[ySize + vSize] = lastUByte

        return nv21
    }

    override fun onDestroy() {
        super.onDestroy()
        cameraExecutor.shutdown()
        nativeWrapper.cleanupNative()
        glRenderer.release()
    }
}
