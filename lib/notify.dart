
import 'dart:async';

import 'package:flutter/services.dart';

class Notify {
  static const MethodChannel _channel =
      const MethodChannel('notify');

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }

  static Future<void> showNotify(String title, String content) async {
    await _channel.invokeMethod('showNotify', {
      'title': title,
      'content': content
    });
  }
}
