# 🌐 Web Viewer Setup Guide - TypeScript Edge Detection Viewer

## 📋 What This Does

The **Web Viewer** is a TypeScript-based HTML5 application that:
- ✅ Displays edge-detected images from your Android app
- ✅ Shows real-time FPS (Frames Per Second) stats
- ✅ Displays resolution information (width × height)
- ✅ Uses HTML5 Canvas for rendering
- ✅ Demonstrates TypeScript project setup and DOM manipulation

---

## 🎯 Quick Start (3 Steps)

### Prerequisites
- ✅ **Node.js** (v14 or higher) - [Download here](https://nodejs.org/)
- ✅ **npm** (comes with Node.js)

Check if installed:
```powershell
node --version   # Should show v14.x.x or higher
npm --version    # Should show 6.x.x or higher
```

---

## 🚀 Step-by-Step Setup

### **Step 1: Navigate to Web Directory**

```powershell
cd "c:\Users\kumat\FLAM ASSIGNMENT\web"
```

---

### **Step 2: Install Dependencies**

```powershell
npm install
```

**Expected output:**
```
added 1 package, and audited 2 packages in 3s
found 0 vulnerabilities
```

This installs:
- TypeScript compiler
- Type definitions

---

### **Step 3: Build TypeScript**

```powershell
npm run build
```

**Expected output:**
```
> edge-detection-web-viewer@1.0.0 build
> tsc
```

This compiles `src/main.ts` → `dist/main.js`

---

### **Step 4: Add Sample Image (3 Options)**

#### **Option A: From Your Android App** ⭐ RECOMMENDED

1. Run your Android edge detection app
2. When edge detection is working, take a screenshot
3. Crop just the edge-detected portion
4. Save as: `web/assets/sample-edge-frame.png`

#### **Option B: Download Sample**

1. Google: "canny edge detection output sample"
2. Download any black/white edge-detected image
3. Save as: `web/assets/sample-edge-frame.png`

#### **Option C: Use Fallback Pattern**

- Delete the `.txt` file in `assets/`
- The viewer will display a grid pattern automatically

---

### **Step 5: Open in Browser**

Simply **double-click**:
```
c:\Users\kumat\FLAM ASSIGNMENT\web\index.html
```

**OR** right-click → Open with → Chrome/Edge/Firefox

---

## 🎨 What You'll See

### Display Elements:

1. **Title**: "Edge Detection Viewer"
2. **Canvas**: Shows the edge-detected image
3. **Stats Overlay** (top-left):
   ```
   FPS: 60
   Resolution: 640 x 480
   ```

### Visual Style:
- Dark theme (background: #121212)
- White text and borders
- Semi-transparent stats overlay
- Responsive canvas

---

## 🛠️ Development Mode (Optional)

### Watch Mode - Auto-Rebuild on Changes

```powershell
npm run watch
```

This runs TypeScript compiler in watch mode:
- Automatically recompiles when you edit `src/main.ts`
- Just refresh browser to see changes
- Press `Ctrl+C` to stop

---

## 📁 Project Structure

```
web/
├── index.html              ← Main HTML file (open this in browser)
├── package.json            ← npm configuration
├── tsconfig.json           ← TypeScript compiler settings
├── src/
│   └── main.ts            ← TypeScript source (YOU EDIT THIS)
├── dist/
│   └── main.js            ← Compiled JavaScript (AUTO-GENERATED)
└── assets/
    └── sample-edge-frame.png  ← Your edge-detected image
```

---

## 🔍 How It Works

### **1. TypeScript Class: `EdgeDetectionViewer`**

```typescript
class EdgeDetectionViewer {
    private canvas: HTMLCanvasElement;  // Drawing surface
    private ctx: CanvasRenderingContext2D;  // 2D rendering context
    private sampleImage: HTMLImageElement;  // Edge-detected image
    private stats: FrameStats;  // FPS, resolution, timestamp
}
```

### **2. Initialization Flow**

```
DOM Ready → Create Viewer → Load Image → Start Animation Loop
```

### **3. Animation Loop**

```
Calculate FPS → Update Stats → Draw Image → RequestAnimationFrame
```

### **4. Stats Update**

- **FPS**: Calculated by counting frames per second
- **Resolution**: Extracted from loaded image dimensions
- Updated every 1000ms (1 second)

---

## 🎯 Key Features Demonstrated

### ✅ TypeScript Setup
- `tsconfig.json` configuration
- ES6 module compilation
- Type-safe interfaces (`FrameStats`)
- Class-based architecture

### ✅ DOM Manipulation
- `getElementById()` to get canvas elements
- `textContent` updates for stats
- Event listeners (DOMContentLoaded, unload)

### ✅ Canvas Rendering
- `getContext('2d')` for drawing
- `drawImage()` for rendering
- `clearRect()` for frame clearing
- Canvas size management

### ✅ Animation Loop
- `requestAnimationFrame()` for smooth updates
- FPS calculation using `performance.now()`
- Proper cleanup with `cancelAnimationFrame()`

### ✅ Error Handling
- `onload` and `onerror` image handlers
- Fallback pattern when image missing
- Graceful degradation

---

## 📊 Testing the Viewer

### Test Checklist:

1. ✅ **Open `index.html` in browser**
   - Title shows: "Edge Detection Viewer"
   - Canvas displays (either image or grid pattern)

2. ✅ **Check Stats Overlay**
   - FPS shows ~60 (or your monitor's refresh rate)
   - Resolution matches your image size
   - Stats update every second

3. ✅ **Test with Different Images**
   - Replace `sample-edge-frame.png` with different sizes
   - Canvas should auto-adjust
   - Resolution should update

4. ✅ **Browser DevTools** (F12)
   - Console shows no errors
   - Check Network tab for image loading
   - Inspect canvas element

5. ✅ **Performance**
   - Smooth animation (60 FPS)
   - No lag or stuttering
   - CPU usage low

---

## 🐛 Troubleshooting

### **Issue: "dist/main.js not found"**

**Solution:**
```powershell
cd "c:\Users\kumat\FLAM ASSIGNMENT\web"
npm run build
```

### **Issue: "npm not recognized"**

**Solution:**
1. Install Node.js: https://nodejs.org/
2. Restart PowerShell
3. Verify: `node --version`

### **Issue: Image not loading**

**Symptoms:**
- Grid pattern shows instead of image
- Console shows 404 error

**Solution:**
- Check file exists: `web/assets/sample-edge-frame.png`
- Check filename exactly matches (case-sensitive)
- Try absolute path in `main.ts` line 50

### **Issue: Stats not updating**

**Solution:**
- Check browser console (F12) for errors
- Verify `dist/main.js` exists and is recent
- Hard refresh: `Ctrl+Shift+R`

### **Issue: TypeScript errors when building**

**Solution:**
```powershell
# Reinstall dependencies
rm -r node_modules
npm install
npm run build
```

---

## 🎨 Customization Examples

### Change Canvas Size

**Edit `index.html` line 53:**
```html
<canvas id="viewerCanvas" width="1280" height="720"></canvas>
```

### Change Stats Position

**Edit `index.html` styles (line 34-41):**
```css
.stats-overlay {
    position: absolute;
    top: 10px;      /* Change to bottom: 10px; */
    right: 10px;    /* Change to right: 10px; */
    ...
}
```

### Change Theme Colors

**Edit `index.html` styles (line 9):**
```css
body {
    background-color: #1a1a2e;  /* Dark blue */
    color: #eee;                /* Off-white */
}
```

### Add More Stats

**Edit `src/main.ts` - Add to FrameStats interface:**
```typescript
interface FrameStats {
    fps: number;
    width: number;
    height: number;
    timestamp: number;
    frameCount: number;  // ADD THIS
}
```

**Update HTML:**
```html
<div id="frameCountDisplay" class="stats-item">Frames: --</div>
```

---

## 🔧 Advanced Features (Optional)

### Live Camera Feed

To connect to your Android app's camera feed in real-time:

1. Set up WebSocket server on Android
2. Stream edge-detected frames as base64
3. Update `main.ts` to receive WebSocket data
4. Replace static image with live stream

### Multiple Images Gallery

To show multiple edge-detected samples:

1. Create array of image paths
2. Add navigation buttons (prev/next)
3. Update `render()` to show selected image

### Export Screenshot

Add download button:

```typescript
downloadCanvas(): void {
    const dataUrl = this.canvas.toDataURL('image/png');
    const link = document.createElement('a');
    link.download = 'edge-detection-output.png';
    link.href = dataUrl;
    link.click();
}
```

---

## ✅ Success Criteria

Your web viewer is working correctly when:

- ✅ Opens in browser without errors
- ✅ Canvas displays edge-detected image or fallback pattern
- ✅ FPS updates every second
- ✅ Resolution matches image dimensions
- ✅ No console errors in browser DevTools
- ✅ Smooth 60 FPS animation
- ✅ Stats overlay visible and readable

---

## 📝 What to Submit (for Assignment)

### Required:

1. **Screenshot** of web viewer running
2. **`dist/main.js`** (compiled TypeScript)
3. **`assets/sample-edge-frame.png`** (your sample image)
4. **Explanation** of:
   - How TypeScript compiles to JavaScript
   - How Canvas API works
   - How FPS is calculated
   - DOM manipulation techniques used

### Bonus Points:

- Add CSS animations
- Implement zoom/pan controls
- Add color scheme toggle
- Create image comparison slider

---

## 📚 Key Concepts Demonstrated

### TypeScript:
- Static typing with interfaces
- Class-based OOP
- Module exports
- Compilation to ES6

### Web APIs:
- Canvas 2D Context
- requestAnimationFrame
- performance.now()
- Image loading
- DOM manipulation

### Best Practices:
- Separation of concerns
- Error handling
- Resource cleanup
- Responsive design

---

## 🎯 Quick Commands Reference

```powershell
# Navigate to web folder
cd "c:\Users\kumat\FLAM ASSIGNMENT\web"

# Install dependencies
npm install

# Build once
npm run build

# Build on file changes (watch mode)
npm run watch

# Check Node.js version
node --version

# Check npm version
npm --version

# Clean and rebuild
rm -r node_modules, dist
npm install
npm run build
```

---

## ⏱️ Estimated Time

- **Setup**: 2 minutes (npm install + build)
- **Get sample image**: 5 minutes
- **Test in browser**: 1 minute
- **Total**: ~8 minutes

---

## 🚀 Next Steps

1. ✅ **Run the web viewer** following steps above
2. ✅ **Take a screenshot** for your assignment
3. ✅ **Explain the code** in your own words
4. ✅ **Show integration** with Android app output

---

**Your web viewer is ready! Just install Node.js, run `npm install`, `npm run build`, and open `index.html`!** 🎉

---

## 📞 Need Help?

Check these files:
- `web/README.md` - Overview
- `package.json` - Dependencies
- `tsconfig.json` - TypeScript config
- Browser DevTools (F12) - Debugging

---

**File Created**: October 8, 2025  
**Project**: Real-Time Edge Detection Viewer  
**Component**: Web Viewer (TypeScript)
