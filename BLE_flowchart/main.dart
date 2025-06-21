import 'dart:async';
import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:url_launcher/url_launcher.dart';

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
    'TraKieu_Apsara_Relief': 'https://www.youtube.com',
    'Tara_Bodhisattva_Statue': 'https://www.facebook.com',
  };

  final Map<String, DateTime> _lastLaunchTimes = {};
  final Duration _cooldown = const Duration(seconds: 30);
  late StreamSubscription<ScanResult> _scanSubscription;

  @override
  void initState() {
    super.initState();
    _startScanning();
  }

  void _startScanning() {
    FlutterBluePlus.startScan(timeout: const Duration(seconds: 0));
    _scanSubscription = FlutterBluePlus.scanResults.listen((results) {
      for (ScanResult result in results) {
        final deviceName = result.device.name;

        if (deviceName.isEmpty) continue;
        if (!beaconToUrl.containsKey(deviceName)) continue;

        final now = DateTime.now();
        final lastLaunch = _lastLaunchTimes[deviceName];

        if (lastLaunch == null || now.difference(lastLaunch) > _cooldown) {
          _lastLaunchTimes[deviceName] = now;
          final url = beaconToUrl[deviceName]!;
          _launchUrl(url);
          print('Launched: $url');
        } else {
          print('Cooldown active for $deviceName');
        }
      }
    });
  }

  Future<void> _launchUrl(String url) async {
    final uri = Uri.parse(url);
    if (await canLaunchUrl(uri)) {
      await launchUrl(uri, mode: LaunchMode.externalApplication);
    } else {
      print('Could not launch $url');
    }
  }

  @override
  void dispose() {
    _scanSubscription.cancel();
    FlutterBluePlus.stopScan();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      home: Scaffold(
        body: Center(
          child: Text('Scanning for cultural beacons...'),
        ),
      ),
    );
  }
}
