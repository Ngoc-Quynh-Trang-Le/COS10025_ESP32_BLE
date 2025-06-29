# :earth_asia: COS10025 BLE-to-Web Cultural Storytelling System

## :page_with_curl: Project Overview

This project was developed for the *COS10025 – Technology in an Indigenous Context Project unit* at Swinburne University of Technology.  
It implements a **non-contact digital storytelling system** to respectfully present cultural narratives from Cham heritage, using:

- **ESP32 BLE beacons** broadcasting artifact identifiers
- A **Flutter Android app** detecting BLE names and opening linked web stories
- Online storytelling pages (text, images, audio) for 2 Cham artifacts:
  - :one: Tra Kieu Apsara Relief
  - :two: Tara Bodhisattva Statue

The solution avoids interaction, pairing, or scanning to align with Cham cultural practices.

---

## :signal_strength: Layer 1: ESP32 BLE Firmware (`main.cpp` via ESP-IDF)

Each ESP32 device functions as a **non-connectable BLE beacon**, configured with:

- **BLE advertising name** hardcoded to either:
  - `"TraKieu_Apsara_Relief"`
  - `"Tara_Bodhisattva_Statue"`
- Advertising mode: `ADV_NONCONN_IND` (non-connectable)
- Broadcast interval: **100–200 ms**
- Flags used:
  - `ESP_BLE_ADV_FLAG_GEN_DISC`
  - `ESP_BLE_ADV_FLAG_BREDR_NOT_SPT`
- **No services, GATT server, or connectable features**
- Runs on ESP32-D0WD-V3 using **ESP-IDF v5.4.1**
- Compiled using `idf.py build`, flashed via `idf.py -p COMx flash`

Deployment details:

- Battery powered (e.g., USB power bank)
- Transparent Acrylic Case for physical protection in public museum settings
- No user action required for operation

---

## :calling: Layer 2: Flutter Mobile App (`main.dart`)

The app (`Cham Story`) is built in Flutter, and functions as a **BLE scanner + URL launcher**.
\
:key: Key behaviors:
\
:one: Scans for BLE advertisements using `flutter_reactive_ble`.
\
:two: Matches `device.name` against a static map:

   ```dart
   const beaconToUrl = {
     'TraKieu_Apsara_Relief': 'https://google.com',
     'Tara_Bodhisattva_Statue': 'https://youtube.com',
   };
   ```

:three: Opens the corresponding URL using `url_launcher` when a match is found.
\
:four: Cooldown enforced to avoid repeat triggers within 30 seconds.
\
:five: Requires **no UI interaction**.
\
:six: Optimized for **offline-friendly sideloading** on Android devices.
\
The app is designed to be sideloaded as an `.apk` file, with no Play Store dependencies.

---

## :art: Design and Cultural Requirements

:heavy_check_mark: No Bluetooth pairing  
:heavy_check_mark: No connectable services or user input  
:heavy_check_mark: Beacon-only broadcast model (non-invasive)  
:heavy_check_mark: Respectful artifact naming only  
:heavy_check_mark: Can be used by multiple visitors simultaneously  
:heavy_check_mark: Artifact proximity awareness without touching the objects  

---

## :arrow_forward: Platform Constraints

| Layer           | Specification                                                   |
|-----------------|-----------------------------------------------------------------|
| BLE Firmware    | ESP32-D0WD-V3, ESP-IDF v5.4.1 (C++, `main.cpp`)                 |
| Mobile App      | Flutter (Android only, `main.dart`)                             |
| Deployment      | Sideload `.apk` on Android devices                              |
| Story Hosting   | Pages hosted at `https://google.com` or `https://youtube.com`   |

---

## :pencil: Notes

- **BLE mode:** `ADV_NONCONN_IND` only
- **No GATT / services / pairing**
- **Human-readable BLE names only**
- **Use only:** `flutter_reactive_ble`, `url_launcher`
- **No use of:** QR, NFC, GPS, user prompts
- **Silent scanning, automatic behavior**
- **Respects cultural protocols for minimal interference**

---