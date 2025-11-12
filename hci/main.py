# main.py
import json
import os
from kivy.app import App
from kivy.lang import Builder
from kivy.properties import StringProperty, ListProperty, BooleanProperty
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.core.window import Window
from kivy.clock import Clock

# Make window a reasonable mobile ratio on desktop for testing
Window.size = (360, 740)

KV = '''
#:import Factory kivy.factory.Factory

<Header@BoxLayout>:
    size_hint_y: None
    height: "56dp"
    padding: "8dp"
    spacing: "8dp"
    canvas.before:
        Color:
            rgba: app.header_bg
        Rectangle:
            pos: self.pos
            size: self.size
    Label:
        text: root.title if hasattr(root, 'title') else ""
        bold: True
        color: app.header_fg
        font_size: "18sp"
        size_hint_x: 0.9
    Button:
        text: "Back"
        size_hint_x: 0.1
        on_release: app.back()

<HomeScreen>:
    name: "home"
    BoxLayout:
        orientation: "vertical"
        Header:
            title: "4-Window App - Home"
        BoxLayout:
            orientation: "vertical"
            padding: "16dp"
            spacing: "12dp"
            Button:
                text: "Open Form"
                size_hint_y: None
                height: "56dp"
                on_release: app.open_screen("form")
            Button:
                text: "Open List"
                size_hint_y: None
                height: "56dp"
                on_release: app.open_screen("list")
            Button:
                text: "Open Settings"
                size_hint_y: None
                height: "56dp"
                on_release: app.open_screen("settings")
            Widget:
            Label:
                text: "Built with Kivy - Mobile friendly"
                size_hint_y: None
                height: "24dp"
                halign: "center"
                valign: "middle"

<FormScreen>:
    name: "form"
    BoxLayout:
        orientation: "vertical"
        Header:
            title: "Form"
        ScrollView:
            do_scroll_x: False
            do_scroll_y: True
            BoxLayout:
                orientation: "vertical"
                padding: "16dp"
                spacing: "12dp"
                size_hint_y: None
                height: self.minimum_height
                MDTextField:
                    id: name_input
                    hint_text: "Full name"
                    multiline: False
                    size_hint_y: None
                    height: "48dp"
                MDTextField:
                    id: email_input
                    hint_text: "Email"
                    multiline: False
                    size_hint_y: None
                    height: "48dp"
                Button:
                    text: "Submit"
                    size_hint_y: None
                    height: "48dp"
                    on_release: root.submit(name_input.text, email_input.text)
                Label:
                    id: msg
                    text: root.message
                    size_hint_y: None
                    height: "28dp"

<ListScreen>:
    name: "list"
    BoxLayout:
        orientation: "vertical"
        Header:
            title: "List"
        BoxLayout:
            orientation: "vertical"
            padding: "12dp"
            spacing: "8dp"
            BoxLayout:
                size_hint_y: None
                height: "48dp"
                spacing: "8dp"
                TextInput:
                    id: new_item
                    hint_text: "New item"
                    multiline: False
                Button:
                    text: "Add"
                    size_hint_x: None
                    width: "80dp"
                    on_release: root.add_item(new_item.text); new_item.text = ""
            ScrollView:
                GridLayout:
                    id: items_container
                    cols: 1
                    size_hint_y: None
                    height: self.minimum_height
                    row_default_height: "48dp"
                    spacing: "6dp"

<SettingsScreen>:
    name: "settings"
    BoxLayout:
        orientation: "vertical"
        Header:
            title: "Settings"
        BoxLayout:
            orientation: "vertical"
            padding: "16dp"
            spacing: "12dp"
            BoxLayout:
                orientation: "horizontal"
                size_hint_y: None
                height: "48dp"
                Label:
                    text: "Enable simple mode"
                    halign: "left"
                Switch:
                    id: toggle_mode
                    active: app.simple_mode
                    on_active: app.set_simple_mode(self.active)
            Button:
                text: "Clear all saved items"
                size_hint_y: None
                height: "48dp"
                on_release: root.clear_items()
            Label:
                text: "App data stored in local data.json"
                size_hint_y: None
                height: "24dp"
                halign: "center"
'''

# A few small helpers to supply MDTextField like style without importing kivymd
from kivy.uix.textinput import TextInput
from kivy.uix.switch import Switch
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.uix.boxlayout import BoxLayout

class MDTextField(TextInput):
    pass

# Screens
class HomeScreen(Screen):
    pass

class FormScreen(Screen):
    message = StringProperty("")

    def submit(self, name, email):
        name = (name or "").strip()
        email = (email or "").strip()
        if not name:
            self.message = "Name required."
            return
        if "@" not in email or "." not in email:
            self.message = "Enter a valid email."
            return
        # simple "functional" behavior - persist to data store as a new contact
        app = App.get_running_app()
        app.data.setdefault("contacts", [])
        app.data["contacts"].append({"name": name, "email": email})
        app.save_data()
        self.message = "Saved. Go to List to see items."

class ListScreen(Screen):
    def on_pre_enter(self):
        self.refresh_list()

    def refresh_list(self):
        app = App.get_running_app()
        items = app.data.get("items", [])
        container = self.ids.items_container
        container.clear_widgets()
        for idx, it in enumerate(items):
            box = BoxLayout(size_hint_y=None, height="48dp", spacing="8dp")
            lbl = Label(text=it, halign="left", valign="middle")
            lbl.bind(size=lbl.setter('text_size'))
            del_btn = Button(text="Delete", size_hint_x=None, width="100dp")
            del_btn.bind(on_release=lambda btn, i=idx: self.delete_item(i))
            box.add_widget(lbl)
            box.add_widget(del_btn)
            container.add_widget(box)

    def add_item(self, text):
        text = (text or "").strip()
        if not text:
            return
        app = App.get_running_app()
        app.data.setdefault("items", [])
        app.data["items"].append(text)
        app.save_data()
        self.refresh_list()

    def delete_item(self, index):
        app = App.get_running_app()
        items = app.data.get("items", [])
        if 0 <= index < len(items):
            items.pop(index)
            app.save_data()
        Clock.schedule_once(lambda dt: self.refresh_list(), 0.05)

class SettingsScreen(Screen):
    def clear_items(self):
        app = App.get_running_app()
        app.data["items"] = []
        app.save_data()
        # if list screen is active, refresh it
        if app.sm.current == "list":
            app.sm.get_screen("list").refresh_list()

class FourWindowApp(App):
    data_file = "data.json"
    data = {}
    sm = None

    # simple theme-ish properties
    header_bg = ListProperty([0.12, 0.5, 0.8, 1])
    header_fg = ListProperty([1, 1, 1, 1])
    simple_mode = BooleanProperty(False)

    def build(self):
        # load kv
        Builder.load_string(KV)
        # create screens
        self.sm = ScreenManager()
        self.sm.add_widget(HomeScreen(name="home"))
        self.sm.add_widget(FormScreen(name="form"))
        self.sm.add_widget(ListScreen(name="list"))
        self.sm.add_widget(SettingsScreen(name="settings"))

        # load data
        self.load_data()
        # apply simple mode if saved
        self.simple_mode = self.data.get("settings", {}).get("simple_mode", False)
        self.apply_theme()
        return self.sm

    def open_screen(self, name):
        if name in self.sm.screen_names:
            self.sm.current = name

    def back(self):
        # if not on home, go home; else exit app
        if self.sm.current != "home":
            self.sm.current = "home"
        else:
            self.stop()

    def load_data(self):
        try:
            if os.path.exists(self.data_file):
                with open(self.data_file, "r", encoding="utf-8") as f:
                    self.data = json.load(f)
            else:
                self.data = {"items": [], "contacts": [], "settings": {}}
        except Exception as e:
            print("Failed to load data:", e)
            self.data = {"items": [], "contacts": [], "settings": {}}

    def save_data(self):
        try:
            with open(self.data_file, "w", encoding="utf-8") as f:
                json.dump(self.data, f, indent=2, ensure_ascii=False)
        except Exception as e:
            print("Failed to save data:", e)

    def set_simple_mode(self, val: bool):
        self.simple_mode = bool(val)
        self.data.setdefault("settings", {})["simple_mode"] = self.simple_mode
        self.save_data()
        self.apply_theme()

    def apply_theme(self):
        # change header color for a very simple theme swap
        if self.simple_mode:
            self.header_bg = [0.1, 0.1, 0.12, 1]
            self.header_fg = [1, 1, 1, 1]
        else:
            self.header_bg = [0.12, 0.5, 0.8, 1]
            self.header_fg = [1, 1, 1, 1]

if __name__ == "__main__":
    FourWindowApp().run()
