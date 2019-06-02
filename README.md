# electron-panel-window

Enables creating a browser window in Electron that behaves like a [Panel](https://developer.apple.com/documentation/appkit/nspanel). Panels are typically used for auxillary windows and do not activate the application – as such they can appear ontop of other apps in the same way as Spotlight or 1Password, for example.

## Usage

Use `PanelWindow` as you would [BrowserWindow](https://electronjs.org/docs/api/browser-window). All of the methods exposed in this module **must be used** on the main process. Using the methods in a renderer process will result in your app crashing.

```javascript
import { PanelWindow } from 'electron-panel-window';

const win = new PanelWindow({
  width: 800,
  height: 600,
  show: false,
});

// the window will show without activating the application
win.show();
```

You can also access the utility methods directly:

```javascript
import { remote } from 'electron';
import { makePanel, makeKeyWindow } from 'electron-panel-window';

const currentWindow = remote.getCurrentWindow();

// convert the window to an NSPanel
makePanel(currentWindow);

// focus the window without activating the application
makeKeyWindow(currentWindow);
```

## Development

To compile the extension for the first time, run

```bash
$ yarn
$ yarn install
```

All subsequent builds only need `yarn build`. Tests run in Spectron:

```bash
$ yarn test
```

## Contributing

This project is built on top of [Abstract](https://www.goabstract.com)'s work.

## Details

File | Contents
-------------|----------------
`NativeExtension.cc` | Represents the top level of the module. C++ constructs that are exposed to javascript are exported here
`functions.cc` | The meat of the extension
`index.js` | The main entry point for the node dependency
`binding.gyp` | Describes the node native extension to the build system (`node-gyp`). If you add source files to the project, you should also add them to the binding file.

## License

This project is under MIT.
See [LICENSE](https://github.com/qazbnm456/electron-panel-window/blob/master/LICENSE)
