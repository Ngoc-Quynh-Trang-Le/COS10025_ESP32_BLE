# COS10025 BLE-to-Web Cultural Storytelling System

## Project Overview

This project was developed for the **COS10025 – Technology in an Indigenous Context** unit at Swinburne University of Technology.  
It implements a **non-contact digital storytelling system** to respectfully present cultural narratives from Cham heritage, using:

- **ESP32 BLE beacons** broadcasting artifact identifiers
- A **Flutter Android app** detecting BLE names and opening linked web stories
- Online storytelling pages (text, images, audio)

The solution avoids interaction, pairing, or scanning to align with Cham cultural practices.

---

## ESP32 BLE Firmware (main.cpp via ESP-IDF)

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
- Cased for physical protection in public museum settings
- No user action required for operation

---

## Flutter Mobile App (`main.dart`)

The app (`Cham Story`) is built in Flutter, and functions as a **BLE scanner + URL launcher**.

Key behaviors:

1. Scans for BLE advertisements using `flutter_blue_plus`
2. Matches `advertisement.advName` against a static map:
   ```dart
   const beaconToUrl = {
     'TraKieu_Apsara_Relief': 'https://facebook.com',
     'Tara_Bodhisattva_Statue': 'https://youtube.com',
   };
   ```
3. On match: launches the linked cultural webpage via `url_launcher`
4. Cooldown enforced to avoid repeat triggers within 30 seconds
5. Requires **no UI interaction** (runs in background silently)
6. Optimized for **offline-friendly sideloading** on Redmi A2 devices

---

## Design and Cultural Requirements

✅ No Bluetooth pairing  
✅ No connectable services or user input  
✅ Beacon-only broadcast model (non-invasive)  
✅ Respectful artifact naming only  
✅ Can be used by multiple visitors simultaneously  
✅ Artifact proximity awareness without touching the objects  

---

## Platform Constraints

| Layer           | Specification                                                   |
|-----------------|-----------------------------------------------------------------|
| BLE Firmware    | ESP-IDF v5.4.1 (C++ - `main.cpp`)                               |
| Mobile App      | Flutter (Android only, `main.dart`)                             |
| Deployment      | Sideload `.apk` on Redmi A2                                     |
| Story Hosting   | Pages hosted at `https://facebook.com` or `https://youtube.com` |

---

## Notes for Copilot / GPT Agents

- **BLE mode:** `ADV_NONCONN_IND` only
- **No GATT / services / pairing**
- **Human-readable BLE names only**
- **Use only:** `flutter_blue_plus`, `url_launcher`
- **No use of:** QR, NFC, GPS, user prompts
- **Silent scanning, automatic behavior**
- **Respects cultural protocols for minimal interference**

```

---

## ✅ Placement Guide

| File            | Action                                                          |
|-----------------|-----------------------------------------------------------------|
| `README.md`     | Place the full Markdown block above                             |
| `main.cpp`      | Paste C++-style comment block at the top (`/* ... */`)          |
| `main.dart`     | Paste condensed comment block (`///`) in first 30 lines         |
| `pubspec.yaml`  | Ensure dependencies listed: `flutter_blue_plus`, `url_launcher` |

---