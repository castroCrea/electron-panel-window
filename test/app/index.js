var app = require("electron").app;
var globalShortcut = require("electron").globalShortcut;
var BrowserWindow = require("electron").BrowserWindow;
var PanelWindow = require("../../").PanelWindow;

var mainWindow = null;

app.on("ready", function () {
  mainWindow = new PanelWindow({
    center: true,
    width: 800,
    height: 600,
    minHeight: 100,
    minWidth: 100,
    show: false,
    transparent: true,
    darkTheme: true,
    webPreferences: {
      preload: `${__dirname}/preload.js`,
      nodeIntegration: true,
      enableRemoteModule: true,
      backgroundThrottling: false,
      webSecurity: false, // iframe external href
    },
  });
  mainWindow.loadURL(`file://${__dirname}/index.html`);
  win = new BrowserWindow({
    frame: false,
    // vibrancy: "sidebar",
    backgroundColor: "#252428",
    titleBarStyle: "hidden",
    minWidth: 700,
    minHeight: 500,
    darkTheme: true,
    worldSafeExecuteJavaScript: true,
    webPreferences: {
      preload: `${__dirname}/preload.js`,
      nodeIntegration: true,
      plugins: true,
      enableRemoteModule: true,
      backgroundThrottling: false,
      webSecurity: false, // iframe external href
    },
    trafficLightPosition: {
      x: 15,
      y: 20,
    },
  });
  win.webContents.setAudioMuted(true);
  win.loadURL(`file://${__dirname}/index.html`);
  mainWindow.on("closed", () => (mainWindow = null));
  win.on("closed", () => (mainWindow = null));
  mainWindow.on("ready-to-show", () => {
    mainWindow.show();
  });
  win.on("ready-to-show", () => {
    win.show();
  });
  globalShortcut.register("CommandOrControl+L", () => {
    if (mainWindow.isVisible()) {
      mainWindow.hide();
    } else {
      mainWindow.show();
    }
  });
  globalShortcut.register("CommandOrControl+K", () => {
    mainWindow.setSize(100, 200, true);
  });
});
