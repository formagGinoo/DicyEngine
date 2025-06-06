#include "include/cef_app.h"
#include "include/wrapper/cef_library_loader.h"

#include "OSRCefApp.h"

// When generating projects with CMake the CEF_USE_SANDBOX value will be defined
// automatically. Pass -DUSE_SANDBOX=OFF to the CMake command-line to disable
// use of the sandbox.
#if defined(CEF_USE_SANDBOX)
#include "include/cef_sandbox_mac.h"
#endif

// Entry point function for sub-processes.
int main(int argc, char* argv[]) {
    #if defined(CEF_USE_SANDBOX)
    // Initialize the macOS sandbox for this helper process.
    CefScopedSandboxContext sandbox_context;
    if (!sandbox_context.Initialize(argc, argv)) {
        return 1;
    }
    #endif

    // Load the CEF framework library at runtime instead of linking directly
    // as required by the macOS sandbox implementation.
    CefScopedLibraryLoader libraryLoader;
    if (!libraryLoader.LoadInHelper()) {
        return 1;
    }

    // Provide CEF with command-line arguments.
    CefMainArgs mainArgs(argc, argv);

    const CefRefPtr<OSRCefApp> osrApp(new OSRCefApp);

    // Execute the sub-process.
    return CefExecuteProcess(mainArgs, osrApp.get(), nullptr);
}
