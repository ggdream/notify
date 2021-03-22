#include "include/notify/notify_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>
#include <libnotify/notify.h>

#define NOTIFY_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), notify_plugin_get_type(), \
                              NotifyPlugin))

struct _NotifyPlugin {
  GObject parent_instance;
};

G_DEFINE_TYPE(NotifyPlugin, notify_plugin, g_object_get_type())

// Called when a method call is received from Flutter.
static void notify_plugin_handle_method_call(
    NotifyPlugin* self,
    FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);
  FlValue* args = fl_method_call_get_args(method_call);

  if (strcmp(method, "getPlatformVersion") == 0) {
    struct utsname uname_data = {};
    uname(&uname_data);
    g_autofree gchar *version = g_strdup_printf("Linux %s", uname_data.version);
    g_autoptr(FlValue) result = fl_value_new_string(version);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  } else if (strcmp(method, "showNotify") == 0) {
    FlValue* title = fl_value_lookup_string(args, "title");
    FlValue* content = fl_value_lookup_string(args, "content");

    notify_init("Simple");
    NotifyNotification *n = notify_notification_new(fl_value_get_string(title), fl_value_get_string(content), 0);
    notify_notification_set_timeout(n, 10000);
    notify_notification_show(n, 0);
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void notify_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(notify_plugin_parent_class)->dispose(object);
}

static void notify_plugin_class_init(NotifyPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = notify_plugin_dispose;
}

static void notify_plugin_init(NotifyPlugin* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  NotifyPlugin* plugin = NOTIFY_PLUGIN(user_data);
  notify_plugin_handle_method_call(plugin, method_call);
}

void notify_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  NotifyPlugin* plugin = NOTIFY_PLUGIN(
      g_object_new(notify_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "notify",
                            FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);

  g_object_unref(plugin);
}
