extern crate gio;
extern crate gtk;
extern crate web_view;

use gio::prelude::*;
use gtk::prelude::*;
use std::cell::RefCell;
use std::rc::Rc;
use web_view::*;


fn main() {
    // Initialize GTK
    gtk::init().expect("Failed to initialize GTK.");

    // Create the application
    let app = gtk::Application::new(
        Some("com.example.web_browser"),
        gio::ApplicationFlags::FLAGS_NONE,
    )
    .expect("Initialization failed...");

    // Connect to activate event
    app.connect_activate(|app| {
        // Create the main window
        let window = gtk::ApplicationWindow::new(app);
        window.set_default_size(800, 600);

        // Create notebook for tabs
        let notebook = gtk::Notebook::new();
        window.add(&notebook);

        // Connect signals for keyboard shortcuts
        let window_clone = window.clone();
        window.connect_key_press_event(move |_, key| {
            if key.get_keyval() == gdk::keys::constants::t && key.get_state().contains(gdk::ModifierType::CONTROL_MASK) {
                let tab_label = gtk::Label::new("New Tab");
                let webview = Rc::new(RefCell::new(web_view::builder().title("New Tab").build().unwrap()));
                let tab_content = gtk::Box::new(gtk::Orientation::Vertical, 0);
                tab_content.pack_start(&webview.borrow().gtk_widget().clone(), true, true, 0);
                let close_button = gtk::Button::new_with_label("x");
                let notebook_clone = notebook.clone();
                close_button.connect_clicked(move |_| {
                    notebook_clone.remove_page(Some(notebook_clone.page_num(&tab_content)));
                });
                tab_content.pack_end(&close_button, false, true, 0);
                notebook.append_page(&tab_content, Some(&tab_label));
                webview.borrow().navigate("https://www.google.com").unwrap();
                window_clone.show_all();
                Inhibit(true)
            } else if key.get_keyval() == gdk::keys::constants::minus && key.get_state().contains(gdk::ModifierType::CONTROL_MASK) {
                if let Some(page_num) = notebook.get_current_page() {
                    notebook.remove_page(Some(page_num));
                    window_clone.show_all();
                }
                Inhibit(true)
            } else {
                Inhibit(false)
            }
        });

        // Show everything
        window.show_all();
    });

    // Run the application
    app.run(&[]);
}
