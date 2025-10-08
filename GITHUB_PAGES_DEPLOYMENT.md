# 🌐 GitHub Pages Deployment Guide

## ✅ Files Deployed

The web viewer has been deployed to the `docs/` folder and pushed to GitHub.

**Deployment Structure:**
```
docs/
├── index.html          # Web interface
├── dist/
│   └── main.js        # Compiled JavaScript
├── assets/
│   └── sample-edge-frame.png
└── README.md          # Documentation
```

---

## 🚀 Enable GitHub Pages (Manual Steps)

### **Step 1: Go to Repository Settings**
1. Open: https://github.com/ak4895/Real-Time-Edge-Detection-Viewer-or-FLAM-Assignment
2. Click **"Settings"** tab (top right)

### **Step 2: Enable GitHub Pages**
1. Scroll down to **"Pages"** in the left sidebar
2. Under **"Source"**, select:
   - **Source**: `Deploy from a branch`
   - **Branch**: `main`
   - **Folder**: `/docs`
3. Click **"Save"**

### **Step 3: Wait for Deployment**
- GitHub will build and deploy (takes 1-2 minutes)
- A green checkmark will appear when ready
- You'll see: **"Your site is live at..."**

### **Step 4: Access Your Live Web Viewer**
Your web viewer will be available at:
```
https://ak4895.github.io/Real-Time-Edge-Detection-Viewer-or-FLAM-Assignment/
```

---

## 🔧 Alternative: GitHub Actions (Already Set Up)

I've also created a GitHub Actions workflow (`.github/workflows/deploy.yml`) that will automatically deploy on every push.

### **To Use GitHub Actions Instead:**
1. Go to Settings → Pages
2. Under **"Source"**, select: `GitHub Actions`
3. The workflow will run automatically on the next push

---

## ✨ Features of Deployed Web Viewer

Once live, users can:
- ✅ Use their webcam for live edge detection
- ✅ Upload images for edge detection
- ✅ See real-time FPS counter
- ✅ Experience Sobel algorithm in action

---

## 🔄 Update Deployment

Whenever you update the web viewer:

```powershell
cd "c:\Users\kumat\FLAM ASSIGNMENT"

# 1. Make changes to web/src/main.ts or web/index.html

# 2. Recompile TypeScript
cd web
npx tsc

# 3. Copy updated files to docs/
cd ..
Copy-Item "web\index.html" "docs\index.html" -Force
Copy-Item "web\dist\main.js" "docs\dist\main.js" -Force

# 4. Commit and push
git add docs/
git commit -m "Update web viewer"
git push
```

GitHub Pages will automatically update (takes 1-2 minutes).

---

## 🎨 Customization

### **Add a Custom Domain** (Optional)
1. Go to Settings → Pages
2. Under "Custom domain", enter your domain
3. Follow DNS configuration instructions

### **HTTPS Enforcement**
- Automatically enabled by GitHub Pages
- Required for webcam access (getUserMedia API)

---

## 📊 Monitoring

### **Check Deployment Status:**
1. Go to repository → **Actions** tab
2. See deployment workflow runs
3. Green checkmark = success ✅
4. Red X = failed ❌ (click for logs)

### **View Live Logs:**
```
Repository → Actions → Latest workflow run → deploy
```

---

## 🐛 Troubleshooting

### **Issue: Page shows 404**
- **Solution**: Make sure you selected `/docs` folder in Settings → Pages

### **Issue: Webcam not working**
- **Cause**: HTTP instead of HTTPS
- **Solution**: GitHub Pages uses HTTPS automatically, should work

### **Issue: "exports is not defined" error**
- **Status**: ✅ Already fixed
- **Solution**: Removed `Object.defineProperty(exports, ...)` from main.js

### **Issue: Deployment not updating**
- **Solution**: Hard refresh browser (Ctrl+Shift+R)
- **Or**: Wait 2-3 minutes for cache to clear

---

## 📱 Share Your Project

Once deployed, share the live demo:

**Live Demo**: https://ak4895.github.io/Real-Time-Edge-Detection-Viewer-or-FLAM-Assignment/

**Repository**: https://github.com/ak4895/Real-Time-Edge-Detection-Viewer-or-FLAM-Assignment

**Add to Resume/Portfolio:**
```
Real-Time Edge Detection Viewer
- Multi-platform computer vision app (Android + Web)
- Live demo: https://ak4895.github.io/Real-Time-Edge-Detection-Viewer-or-FLAM-Assignment/
- Code: https://github.com/ak4895/Real-Time-Edge-Detection-Viewer-or-FLAM-Assignment
```

---

## ✅ Checklist

- [x] Created `docs/` folder
- [x] Copied `index.html` and `main.js`
- [x] Added GitHub Actions workflow
- [x] Pushed to GitHub
- [ ] **YOU DO**: Enable GitHub Pages in Settings
- [ ] **YOU DO**: Test live URL
- [ ] **YOU DO**: Grant webcam permission and test

---

**Next Step**: Go to repository Settings → Pages and enable GitHub Pages with `/docs` folder!
