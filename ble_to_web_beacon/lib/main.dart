/// COS10025 BLE-to-Web Cultural Storytelling System
/// Da Nang Museum of Cham Sculpture
/// Flutter mobile app to detect ESP32 BLE beacons near Cham artifacts,
/// and launch corresponding webpage in the mobile browser for storytelling.
/// 
/// Layer 2: Mobile Flutter App 
///   - Architecture: BLE passive scanning (flutter_reactive_ble)
///   - Output: Opens artifact webpage via external browser (url_launcher)
///   - Design: Fully automatic, culturally respectful, no tap, no pairing
///
/// NOTE: BLE beacon names must match exactly with those hardcoded below.
///       Artifact story content must be hosted and accessible via Android browser.

library;

import 'dart:async';
import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';     // For passive BLE scanning
import 'package:url_launcher/url_launcher.dart';                     // For launching web stories in default browser
import 'package:permission_handler/permission_handler.dart' as perm; // For requesting runtime Android permissions

void main() {
  runApp(const MyApp()); // Start Flutter UI wrapper (minimal)
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  /// 🔗 Mapping Dictionary
  /// Matches BLE device name (broadcasted by ESP32 beacons near artifacts)
  /// to their associated storytelling URLs (hosted externally)
  static const beaconToUrl = {
    'TraKieu_Apsara_Relief': 'https://www.google.com',    // Replace with production URL e.g., 'https://museum.vn/apsara'
    'Tara_Bodhisattva_Statue': 'https://www.youtube.com', // Replace with production URL e.g., 'https://museum.vn/tara'
  };

  final Map<String, DateTime> _lastLaunchTimes = {};      // Used to suppress rapid repeat launches per device
  final Duration _cooldown = const Duration(seconds: 30); // Cooldown duration to prevent spamming (adjustable per field testing)

  late StreamSubscription<DiscoveredDevice> _scanSubscription; // Active subscription to BLE scanning stream
  final FlutterReactiveBle _ble = FlutterReactiveBle();        // BLE plugin instance

  String? _lastDetectedDeviceName; // Cache of last valid artifact name detected (used for display + logic filtering)

  @override
  void initState() {
    super.initState();
    _startScanning(); // Initiate BLE scan on app startup
  }

  /// 📶 Start scanning for advertising BLE packets (broadcasted by ESP32)
  /// Matches device names against known Cham artifact beacons
  void _startScanning() async {
    print('Starting BLE scan...');
    try {
      // Request runtime permissions required for BLE scanning on Android
      var loc = await perm.Permission.location.request();
      var scan = await perm.Permission.bluetoothScan.request();
      var conn = await perm.Permission.bluetoothConnect.request();
      print('Permission status: location=$loc, bluetoothScan=$scan, bluetoothConnect=$conn');
    } catch (e) {
      // Catch and report if permission plugin fails (rare case)
      print('WARNING: permission_handler not available or failed: $e');
    }

    // Begin scanning for all advertising devices with high frequency (lowLatency scan mode)
    _scanSubscription = _ble.scanForDevices(withServices: [], scanMode: ScanMode.lowLatency).listen((device) {
      print('Device: ${device.id} Name: ${device.name}');
      
      // Only proceed if device name is non-empty and recognized in our mapping dictionary
      if (device.name.isNotEmpty && beaconToUrl.containsKey(device.name)) {
        setState(() {
          _lastDetectedDeviceName = device.name;
        });

        final now = DateTime.now();
        final lastLaunch = _lastLaunchTimes[device.name];

        // Launch storytelling URL if cooldown has expired or first-time detection
        if (lastLaunch == null || now.difference(lastLaunch) > _cooldown) {
          _lastLaunchTimes[device.name] = now;
          print('MATCHED: ${device.name} - launching URL after delay');

          // Add 2-second delay to avoid race condition from rapid multiple matches
          Future.delayed(const Duration(seconds: 2), () {
            if (_lastDetectedDeviceName == device.name) {
              _launchUrl(beaconToUrl[device.name]!);
            }
          });
        } else {
          print('MATCHED: ${device.name} - cooldown active, not launching');
        }
      }
    }, onError: (e) {
      print('BLE scan error: $e'); // Handle BLE scan failure silently
    });
  }

  /// 🌐 Launch associated artifact story in external browser
  /// Ensures non-intrusive, respectful delivery aligned with museum experience
  Future<void> _launchUrl(String url) async {
    final uri = Uri.parse(url);
    try {
      final launched = await launchUrl(uri, mode: LaunchMode.externalApplication);
      if (!launched) {
        print('launchUrl returned false for: $url');
      }
    } catch (e) {
      print('Exception launching URL $url: $e'); // Silent error logging
    }
  }

  @override
  void dispose() {
    _scanSubscription.cancel(); // Stop BLE scan when widget is destroyed
    _ble.deinitialize(); // Deinit BLE engine safely
    super.dispose();
  }

  /// 📱 Simple UI text feedback (no interactivity)
  /// Shows current scan status or matched artifact being opened
  @override
  Widget build(BuildContext context) {
    String displayText;
    if (_lastDetectedDeviceName != null) {
      final artifactName = _lastDetectedDeviceName!.replaceAll('_', ' '); // Improve readability for end users
      displayText = 'Opening $artifactName Artifact Story...';
    } else {
      displayText = 'Scanning for BLE beacons...';
    }

    return MaterialApp(
      home: Scaffold(
        body: Center(
          child: Text(displayText),
        ),
      ),
    );
  }
}
