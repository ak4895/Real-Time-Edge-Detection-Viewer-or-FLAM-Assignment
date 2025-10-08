# Web Viewer - Setup Instructions

## Quick Setup

### 1. Install Dependencies
```bash
cd web
npm install
```

### 2. Build TypeScript
```bash
npm run build
```

### 3. Create Assets Directory
```bash
mkdir assets
```

### 4. Add Sample Image (Optional)
Place a sample edge-detected image at:
```
web/assets/sample-edge-frame.png
```

If no image is provided, a fallback grid pattern will be displayed.

### 5. Run Local Server
```bash
npx http-server
```

### 6. Open Browser
Navigate to: `http://localhost:8080`

## What You Should See

- Edge detection viewer with canvas
- FPS counter (simulated)
- Resolution display
- Sample image or fallback pattern

## File Structure

```
web/
├── index.html          # Main HTML page
├── package.json        # NPM configuration
├── tsconfig.json       # TypeScript configuration
├── src/
│   └── main.ts        # TypeScript source
├── dist/              # Compiled JavaScript (generated)
│   └── main.js
└── assets/            # Images (create this)
    └── sample-edge-frame.png (optional)
```

## Development

Watch mode for auto-compilation:
```bash
npm run watch
```

## Troubleshooting

**Error: "Cannot find module 'typescript'"**
```bash
npm install
```

**Error: "Permission denied"**
```bash
npx http-server --port 8081
```

**Browser shows blank page**
- Check browser console (F12) for errors
- Verify dist/main.js exists
- Check that http-server is running

**Sample image not loading**
- Place image in `web/assets/sample-edge-frame.png`
- Or use any edge-detected image
- Fallback pattern will show if image missing

## Notes

- This is a static viewer for demonstration
- In production, you would stream frames from the Android app
- FPS and stats are simulated for demonstration
- Uses vanilla TypeScript (no frameworks required)
