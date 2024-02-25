import sys
from PyQt5.QtCore import QUrl, Qt
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QLineEdit, QPushButton, QWidget, QTabWidget, QShortcut, QDialog
from PyQt5.QtWebEngineWidgets import QWebEngineView, QWebEnginePage, QWebEngineSettings, QWebEngineProfile

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

class NovaNav(QWebEngineView):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("NovaNav - Super Lightweight Browser")
        self.setGeometry(100, 100, 800, 600)

        self.tab_widget = QTabWidget()
        self.tab_widget.setTabsClosable(True)
        self.tab_widget.tabCloseRequested.connect(self.close_tab)
        self.tab_widget.setMinimumWidth(200)  # Set a minimum width for the tabs

        self.layout = QVBoxLayout()
        self.layout.addWidget(self.tab_widget)
        self.setLayout(self.layout)

        self.url_input_dialog = URLInputDialog()
        self.url_input_dialog.ok_button.clicked.connect(self.open_new_tab)

        self.shortcut_new_tab = QShortcut("Ctrl+T", self)
        self.shortcut_new_tab.activated.connect(self.show_url_input_dialog)

        self.create_new_tab("https://www.google.com")

        # Set permissions and settings
        self.set_permissions_and_settings()

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

        # Connect linkClicked signal to handle_link_click method
        self.page().linkClicked.connect(self.handle_link_click)  

    def create_new_tab(self, url):
        if not url.startswith("http"):
            url = "http://" + url
        browser = QWebEngineView()
        browser.setUrl(QUrl(url))
        browser.page().profile().setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36") # Customize the user agent to avoid blocking by some websites
        browser.titleChanged.connect(lambda title, browser=browser: self.set_tab_title(browser, title[:20]))  # Limit title to 20 characters
        browser.page().fullScreenRequested.connect(lambda request: request.accept())
        browser.page().setZoomFactor(0.65)  # Set default zoom factor to 65%
        
        self.tab_widget.addTab(browser, "")

    def handle_link_click(self, url):
        # Get the HTML content of the current page
        html_content = self.page().toHtml(self.handle_html_content)

    def handle_html_content(self, html):
        # Check if the HTML contains the target="_blank" attribute
        if '_blank' in html:
            # Open the link in a new tab
            self.create_new_tab(url.toString())
        else:
            # Open the link in the same tab
            self.page().setUrl(url)

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

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = NovaNav()
    window.show()
    sys.exit(app.exec_())
