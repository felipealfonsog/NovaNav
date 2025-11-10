use gtk::prelude::*;
use gtk::{Application, ApplicationWindow, Notebook, Label, AccelGroup, AccelFlags};

fn main() {
    // Initialize GTK application
    let app = Application::new(
        Some("com.example.web_browser"),
        gio::ApplicationFlags::empty(),
    )
    .expect("Failed to initialize GTK application");

    // Connect to the activate signal to create the window
    app.connect_activate(|app| {
        // Create the main window
        let window = ApplicationWindow::new(app);
        window.set_title("NovaNav");
        window.set_default_size(600, 400);

        // Create a notebook to hold the tabs
        let notebook = Notebook::new();

        // Create an accelerator group to handle keyboard shortcuts
        let accel_group = AccelGroup::new();
        window.add_accel_group(&accel_group);

        // Shortcut to open a new tab (Ctrl + T)
        let accel_key_t = gdk::keys::constants::T;
        accel_group.connect_accel_closure(Some(&accel_key_t), false, move |_| {
            // Create a new label for the tab
            let label = Label::new(Some("New Tab"));

            // Create a new page for the notebook
            let tab_content = gtk::Box::new(gtk::Orientation::Vertical, 0);
            tab_content.add(&Label::new(Some("New tab content")));

            // Append the new tab to the notebook
            notebook.append_page(&tab_content, Some(&label));

            // Show all widgets
            window.show_all();

            // Return true to stop further handling
            Inhibit(false)
        });

        // Shortcut to close the current tab (Ctrl + W)
        let accel_key_w = gdk::keys::constants::W;
        accel_group.connect_accel_closure(Some(&accel_key_w), false, move |_| {
            // Get the current page index
            if let Some(page_num) = notebook.current_page() {
                // Remove the current page from the notebook
                notebook.remove_page(Some(page_num));
            }

            // Return true to stop further handling
            Inhibit(false)
        });

        // Add the notebook to the window (only once)
        window.set_child(Some(&notebook));

        // Show all widgets
        window.show_all();
    });

    // Run the application
    app.run();
}
