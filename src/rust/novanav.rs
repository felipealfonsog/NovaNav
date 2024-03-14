use gtk::prelude::*;
use gio::prelude::*;

fn main() {
    gtk::init().expect("Failed to initialize GTK.");

    let app = gtk::Application::new(
        Some("com.example.web_browser"),
        gio::ApplicationFlags::empty(),
    )
    .expect("Failed to initialize the application.");

    app.connect_activate(|app| {
        let window = gtk::ApplicationWindow::new(app);
        window.set_default_size(800, 600);

        let notebook = gtk::Notebook::new();
        window.add(&notebook);

        window.connect_key_release_event(move |_, event| {
            if let Some(key) = event.key() {
                if let Some(state) = event.state() {
                    if key == gdk::keys::constants::T && state.contains(gdk::ModifierType::CONTROL_MASK) {
                        let tab_label = gtk::Label::new(Some("New Tab"));
                        let tab_content = gtk::Box::new(gtk::Orientation::Vertical, 0);
                        notebook.append_page(&tab_content, Some(&tab_label));
                        window.show_all();
                        return Inhibit(true);
                    } else if key == gdk::keys::constants::minus && state.contains(gdk::ModifierType::CONTROL_MASK) {
                        if let Some(page_num) = notebook.current_page() {
                            notebook.remove_page(Some(page_num));
                            window.show_all();
                        }
                        return Inhibit(true);
                    }
                }
            }
            Inhibit(false)
        });

        window.show_all();
    });

    app.run(&[]);
}
