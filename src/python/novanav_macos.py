import sys
from PyQt5.QtCore import QUrl, Qt
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QLineEdit, QPushButton, QWidget, QTabWidget, QShortcut, QDialog, QHBoxLayout, QLabel
from PyQt5.QtWebEngineWidgets import QWebEngineView, QWebEnginePage, QWebEngineSettings

class URLInputDialog(QDialog):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Enter URL")
        layout = QVBoxLayout(self)
        self.url_entry = QLineEdit()
        layout.addWidget(self.url_entry)
        self.ok_button = QPushButton("OK")
        layout.addWidget(self.ok_button)
        self.ok_button.clicked.connect(self.accept)

class NovaNav(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("NovaNav - Super Lightweight Browser")
        self.setGeometry(100, 100, 800, 600)

        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)
        self.layout = QVBoxLayout(self.central_widget)

        self.tab_widget = QTabWidget()
        self.tab_widget.setTabsClosable(True)
        self.tab_widget.tabCloseRequested.connect(self.close_tab)
        self.tab_widget.setMinimumWidth(200)

        # Personaliza el área de contenido de la barra de pestañas

        
        # Personaliza el área de contenido de la barra de pestañas
        self.tab_widget.setStyleSheet("QTabBar::tab { min-width: 20px; padding: 7px; border-top-left-radius: 8px; border-top-right-radius: 8px; }"
                                       "QTabBar::tab:selected { min-width: 20px; padding: 7px; border-top-left-radius: 8px; border-top-right-radius: 8px; }")


        self.layout.addWidget(self.tab_widget)

        self.url_input_dialog = URLInputDialog()
        self.url_input_dialog.ok_button.clicked.connect(self.open_new_tab)

        self.shortcut_new_tab = QShortcut("Ctrl+T", self)
        self.shortcut_new_tab.activated.connect(self.show_url_input_dialog)

        self.shortcut_zoom_in = QShortcut(Qt.CTRL + Qt.Key_Plus, self)
        self.shortcut_zoom_in.activated.connect(self.zoom_in)

        self.shortcut_zoom_out = QShortcut(Qt.CTRL + Qt.Key_Minus, self)
        self.shortcut_zoom_out.activated.connect(self.zoom_out)

        self.shortcut_toggle_titles = QShortcut(Qt.CTRL + Qt.Key_V, self)
        self.shortcut_toggle_titles.activated.connect(self.toggle_titles)

        self.create_new_tab("https://www.google.com")

        self.set_permissions_and_settings()
        self.add_navigation_buttons()
        
        # Establece la altura mínima de la ventana principal
        self.setMinimumHeight(800)  # Ajusta la altura mínima según preferencia


    def add_navigation_buttons(self):
        button_container = QWidget()
        button_layout = QHBoxLayout(button_container)
        
        plus_button = QPushButton("+")
        plus_button.setToolTip("Open Google")
        plus_button.setMaximumWidth(30)  # Adjust button size
        plus_button.setStyleSheet("QPushButton { padding: 10px 2px; }")
        plus_button.clicked.connect(self.open_google_tab)

        backward_button = QPushButton("<")
        backward_button.setToolTip("Back")
        backward_button.setMaximumWidth(30)  # Adjust button size
        backward_button.setStyleSheet("QPushButton { padding: 10px 2px; margin-left: 10px; margin-right: 10px; }")
        backward_button.clicked.connect(self.go_back)

        forward_button = QPushButton(">")
        forward_button.setToolTip("Forward")
        forward_button.setMaximumWidth(30)  # Adjust button size
        forward_button.setStyleSheet("QPushButton { padding: 10px 2px; margin-left: 10px; margin-right: 10px; }")
        forward_button.clicked.connect(self.go_forward)

        credits_button = QPushButton("=")
        credits_button.setToolTip("Credits")
        credits_button.setMaximumWidth(30)  # Adjust button size
        credits_button.setStyleSheet("QPushButton { padding: 10px 2px; margin-left: 10px; margin-right: 10px; }")
        credits_button.clicked.connect(self.show_credits_popup)

        ctrl_v_button = QPushButton("-")
        ctrl_v_button.setToolTip("Ctrl + V")
        ctrl_v_button.setMaximumWidth(30)  # Adjust button size
        ctrl_v_button.setStyleSheet("QPushButton { padding: 10px 2px; }")
        ctrl_v_button.clicked.connect(self.toggle_titles)

        button_layout.addWidget(plus_button)
        button_layout.addWidget(backward_button)
        button_layout.addWidget(forward_button)
        button_layout.addWidget(credits_button)
        button_layout.addWidget(ctrl_v_button)

        # Add a stretch to push buttons to the top
        button_layout.addStretch()

        # Insert the button container into the tab widget's header
        self.tab_widget.setCornerWidget(button_container, Qt.TopRightCorner)




    def open_google_tab(self):
        self.create_new_tab("https://www.google.com")

    def show_credits_popup(self):
        credits_popup = QDialog(self)
        credits_popup.setWindowTitle("Credits")
        credits_popup.setMinimumWidth(400)  # Ajustar el ancho mínimo para mostrar bien el contenido
        credits_layout = QVBoxLayout(credits_popup)
        
        credits_label = QLabel(
            "Credits:\n"
            "Computer Science Engineer: Felipe Alfonso González\n"
            "GitHub: github.com/felipealfonsog\n"
            "\n"
            "--------------------------------------------------\n"
            "This software is licensed under the BSD 3-Clause License.\n"
            "For details, see: https://opensource.org/licenses/BSD-3-Clause\n"
            "\n"
            "Usage restrictions:\n"
            "For any inquiries about usage restrictions, please contact:\n"
            "Felipe Alfonso González\n"
            "Email: f.alfonso@res-ear.ch\n"
            "\n"
            "--------------------------------------------------\n"
            "FAQ:\n"
            "\n"
            "ctrl+t (New tab)\n"
            "ctrl+v (hide tabs for distraction-free)\n"
            "ctrl+q (quit)\n"
        )
        
        credits_label.setAlignment(Qt.AlignCenter)  # Centrar el contenido
        credits_layout.addWidget(credits_label)
        
        credits_popup.exec_()

    def go_back(self):
        current_browser = self.tab_widget.currentWidget()
        if current_browser and current_browser.history().canGoBack():
            current_browser.back()

    def go_forward(self):
        current_browser = self.tab_widget.currentWidget()
        if current_browser and current_browser.history().canGoForward():
            current_browser.forward()

    def set_permissions_and_settings(self):
        settings = QWebEngineSettings.globalSettings()
        settings.setAttribute(QWebEngineSettings.ScreenCaptureEnabled, True)
        settings.setAttribute(QWebEngineSettings.LocalStorageEnabled, True)
        settings.setAttribute(QWebEngineSettings.PluginsEnabled, True)
        settings.setAttribute(QWebEngineSettings.JavascriptEnabled, True)
        settings.setAttribute(QWebEngineSettings.AutoLoadImages, True)
        settings.setAttribute(QWebEngineSettings.JavascriptCanAccessClipboard, True)
        settings.setAttribute(QWebEngineSettings.JavascriptCanOpenWindows, True)
        settings.setAttribute(QWebEngineSettings.XSSAuditingEnabled, True)
        settings.setAttribute(QWebEngineSettings.Accelerated2dCanvasEnabled, True)
        settings.setAttribute(QWebEngineSettings.FullScreenSupportEnabled, True)
        settings.setAttribute(QWebEngineSettings.ErrorPageEnabled, True)

    def create_new_tab(self, url):
        if not url.startswith("http"):
            url = "http://" + url
        browser = ExternalLinkHandler()
        browser.setUrl(QUrl(url))
        browser.page().profile().setHttpUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36") # Customize the user agent to avoid blocking by some websites
        browser.titleChanged.connect(lambda title, browser=browser: self.set_tab_title(browser, title[:20]))  # Limit title to 20 characters
        browser.page().fullScreenRequested.connect(lambda request: request.accept())
        browser.page().urlChanged.connect(self.handle_url_changed)
        browser.page().setZoomFactor(0.65)  # Set default zoom factor to 65%

        self.tab_widget.addTab(browser, "")

    def handle_url_changed(self, url):
        if "_blank" in url.toString():
            current_browser = self.tab_widget.currentWidget()
            if current_browser:
                current_browser.setUrl(url)

    def show_url_input_dialog(self):
        self.url_input_dialog.show()

    def open_new_tab(self):
        url = self.url_input_dialog.url_entry.text()
        if url:
            self.create_new_tab(url)
        self.url_input_dialog.hide()

    def close_tab(self, index):
        self.tab_widget.removeTab(index)

    def set_tab_title(self, browser, title):
        index = self.tab_widget.indexOf(browser)
        self.tab_widget.setTabText(index, title)

    def zoom_in(self):
        current_browser = self.tab_widget.currentWidget()
        if current_browser:
            current_browser.setZoomFactor(current_browser.zoomFactor() + 0.1)

    def zoom_out(self):
        current_browser = self.tab_widget.currentWidget()
        if current_browser:
            current_browser.setZoomFactor(current_browser.zoomFactor() - 0.1)

    def toggle_titles(self):
        for i in range(self.tab_widget.count()):
            browser = self.tab_widget.widget(i)
            title = browser.title() if self.tab_widget.tabBar().isVisible() else self.tab_widget.tabText(i)
            self.set_tab_title(browser, title[:20])
        self.tab_widget.tabBar().setVisible(not self.tab_widget.tabBar().isVisible())

class ExternalLinkHandler(QWebEngineView):
    def createWindow(self, windowType):
        if windowType == QWebEnginePage.WebBrowserTab:
            return self
        return super().createWindow(windowType)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = NovaNav()
    window.show()
    sys.exit(app.exec_())
