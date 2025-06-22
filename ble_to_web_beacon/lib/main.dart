/// COS10025 BLE-to-Web Cultural Storytelling System
/// Flutter app for automatic BLE-triggered Cham artifact storytelling.
/// - Always-on BLE scan (flutter_reactive_ble)
/// - Matches artifact name, launches URL in external browser (url_launcher)
/// - No user interaction, no in-app WebView, silent operation
/// - See README.md for full project details
library;

import 'dart:async';
import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:url_launcher/url_launcher.dart';
import 'package:permission_handler/permission_handler.dart' as perm;

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  static const beaconToUrl = {
    'TraKieu_Apsara_Relief': 'https://www.facebook.com',
    'Tara_Bodhisattva_Statue': 'https://www.youtube.com',
  };

  final Map<String, DateTime> _lastLaunchTimes = {};
  final Duration _cooldown = const Duration(seconds: 30); // Reduced for testing
  late StreamSubscription<DiscoveredDevice> _scanSubscription;
  final FlutterReactiveBle _ble = FlutterReactiveBle();

  @override
  void initState() {
    super.initState();
    _startScanning();
  }

  void _startScanning() async {
    print('Starting BLE scan...');
    try {
      var loc = await perm.Permission.location.request();
      var scan = await perm.Permission.bluetoothScan.request();
      var conn = await perm.Permission.bluetoothConnect.request();
      print('Permission status: location=$loc, bluetoothScan=$scan, bluetoothConnect=$conn');
    } catch (e) {
      print('WARNING: permission_handler not available or failed: $e');
    }
    _scanSubscription = _ble.scanForDevices(withServices: [], scanMode: ScanMode.lowLatency).listen((device) {
      print('Device: ${device.id} Name: ${device.name}');
      if (device.name.isNotEmpty && beaconToUrl.containsKey(device.name)) {
        final now = DateTime.now();
        final lastLaunch = _lastLaunchTimes[device.name];
        if (lastLaunch == null || now.difference(lastLaunch) > _cooldown) {
          _lastLaunchTimes[device.name] = now;
          print('MATCHED: ${device.name} - launching URL');
          _launchUrl(beaconToUrl[device.name]!);
        } else {
          print('MATCHED: ${device.name} - cooldown active, not launching');
        }
      }
    }, onError: (e) {
      print('BLE scan error: $e');
    });
  }

  Future<void> _launchUrl(String url) async {
    final uri = Uri.parse(url);
    try {
      final launched = await launchUrl(uri, mode: LaunchMode.externalApplication);
      if (!launched) {
        print('launchUrl returned false for: $url');
      }
    } catch (e) {
      print('Exception launching URL $url: $e');
    }
  }

  @override
  void dispose() {
    _scanSubscription.cancel();
    _ble.deinitialize();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      home: Scaffold(
        body: Center(
          child: Text('Opening Tra Kieu Apsara Relief Artifact Story...'),
        ),
      ),
    );
  }
}
