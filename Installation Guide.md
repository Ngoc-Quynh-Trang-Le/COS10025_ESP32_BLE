# Installation Guide for BLE-to-Web Cultural Storytelling System

## 1️⃣ Flutter App

### ✅ Change App Name (e.g., `ble_to_web_beacon` → `Cham Story`)

#### 🔧 Step 1: Edit `pubspec.yaml`

Open `pubspec.yaml` and update:

```yaml
name: cham_story
description: A Flutter app for BLE-triggered Cham artifact storytelling.
```

---

#### 🔧 Step 2: Edit `android/app/src/main/AndroidManifest.xml`

Change this line (near the top):

```xml
<application
  android:label="Cham Story"
```

---

#### 🔧 Step 3: Optional — Edit iOS Name (if needed)

Open:

```
ios/Runner/Info.plist
```

Change this line:

```xml
<key>CFBundleName</key>
<string>Cham Story</string>
```

---

### 📱 Install `Cham Story` App on Android Device (Clean + Rebuild)

In terminal (from root of your project):

```bash
flutter clean
flutter pub get
flutter build apk --release
adb install build/app/outputs/flutter-apk/app-release.apk
```

---

## 2️⃣ BLE Beacons

Compile and flash the ESP32 firmware using ESP-IDF.
```idf.py set-target esp32
idf.py build
idf.py -p COM3 flash
idf.py monitor
```

---

### 💻 Open the ESP-IDF Terminal

To use ESP-IDF with VS Code, you need to open the ESP-IDF terminal. This can be done by running the export script in your terminal.

On Linux or macOS, run:

```bash
cd ~/esp/esp-idf
source export.sh
```
On Windows, you can run the export script from PowerShell or CMD:

  - On Windows PowerShell:

    ```powershell
    . $HOME/espressif/esp-idf/export.ps1
    ```

  - On Windows CMD:

    ```cmd
    %USERPROFILE%\espressif\esp-idf\export.bat
    ```

On Git Bash, run:

   ```bash
   source $HOME/espressif/esp-idf/export.sh
   ```

---

### 🔁 Troubleshooting (if squiggles remain)

- Restart VS Code (or press `Ctrl+Shift+P → Reload Window`)

- Manually reconfigure ESP-IDF menuconfig: press `Ctrl+Shift+P → ESP-IDF: Reconfigure ESP-IDF Project`

- If the IntelliSense still says "cannot open source file...":

  Force a rebuild (Clean and rebuild the project):

  ```idf.py fullclean
  idf.py reconfigure
  idf.py build
  ```
  OR
  
  ```idf.py fullclean
  idf.py reconfigure build
  ```

  OR
  
  delete the `build` folder in your project root and reconfigure again.

- Or clear IntelliSense cache:

  - Press `Ctrl+Shift+P`
  - Choose: `C/C++: Reset IntelliSense Database`

- If you still see issues, try deleting the `.vscode` folder in your project root and reconfigure.

---





