# 🚀 Alex Space Shooter // C++ WebAssembly Demo

A minimalist arcade game built from scratch in **C++** using the **Raylib** library, compiled into **WebAssembly (Wasm)** to run natively in the browser and hosted live on **Vercel**.

---

## 🛠️ Tech Stack & Architecture

* **Core Engine:** C++
* **Graphics & Window Management:** Raylib
* **Web Compiler:** Emscripten (Wasm / JavaScript Glue Code)
* **Frontend HUD:** HTML5 Canvas, CSS3 (Cyberpunk / NASA Terminal Theme)
* **Deployment:** Vercel Static Hosting

---

## 🎮 How to Play

* **Move Left:** `A` or `Left Arrow`
* **Move Right:** `D` or `Right Arrow`
* **Objective:** Control the spacecraft and navigate through deep space missions.

---

## ⚙️ Local Development & Compilation

To build and run this project locally, you need the **Emscripten SDK** installed on your machine.

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/YOUR_USERNAME/space-shooter-cpp.git](https://github.com/YOUR_USERNAME/space-shooter-cpp.git)
   cd space-shooter-cpp
Compile C++ to WebAssembly:
Make sure your Emscripten environment is activated in your terminal, then run the compilation command to generate index.js and index.wasm:

Bash
emcc main.cpp -o index.js -s USE_GLFW=3 -s ASYNCIFY -O3 --shell-file index.html
Run a local server:
You can use any simple local server (like Python) to test the game in your browser:

Bash
python -m http.server 8000
Open http://localhost:8000 in your browser.

🌐 Live Demo
Play the live demo directly in your browser via Vercel:
👉 [https://game-space-wine.vercel.app/]

Built with passion, code, and a touch of late-night engineering. 🌌
