import sys
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QLineEdit, QPushButton, QWidget, QTabWidget, QHBoxLayout, QShortcut

from PyQt5.QtWebEngineWidgets import QWebEngineView

class NovaNav(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("NovaNav")
        self.setGeometry(100, 100, 800, 600)

        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)
        self.layout = QVBoxLayout(self.central_widget)

        self.tab_widget = QTabWidget()
        self.layout.addWidget(self.tab_widget)

        self.create_new_tab()

        self.shortcut_new_tab = QShortcut("Ctrl+T", self)
        self.shortcut_new_tab.activated.connect(self.create_new_tab)

        self.shortcut_toggle_sidebar = QShortcut("Ctrl+S", self)
        self.shortcut_toggle_sidebar.activated.connect(self.toggle_sidebar)

        self.sidebar_hidden = True
        self.sidebar_button = QPushButton("=", self)
        self.sidebar_button.clicked.connect(self.toggle_sidebar)
        self.sidebar_button.setVisible(False)

    def create_new_tab(self):
        browser = QWebEngineView()
        browser.setUrl(QtCore.QUrl("http://www.example.com"))
        browser.urlChanged.connect(self.update_url_bar)

        index = self.tab_widget.addTab(browser, "New Tab")
        self.tab_widget.setCurrentIndex(index)

    def update_url_bar(self, url):
        index = self.tab_widget.currentIndex()
        browser = self.tab_widget.widget(index)
        self.setWindowTitle(browser.title())

    def toggle_sidebar(self):
        if self.sidebar_hidden:
            self.layout.addWidget(self.tab_widget)
            self.sidebar_button.setVisible(True)
            self.sidebar_hidden = False
        else:
            self.layout.removeWidget(self.tab_widget)
            self.sidebar_button.setVisible(False)
            self.sidebar_hidden = True

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Equal:
            if self.sidebar_hidden:
                self.toggle_sidebar()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = NovaNav()
    window.show()
    sys.exit(app.exec_())
