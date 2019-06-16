var app = require('electron').app;
var PanelWindow = require('../../').PanelWindow;

var mainWindow = null;

app.on('ready', function () {
  mainWindow = new PanelWindow({
    center: true,
    width: 800,
    height: 600,
    minHeight: 100,
    minWidth: 100,
    show: false,
    webPreferences: {
      preload: `${__dirname}/preload.js`,
    },
  });
  mainWindow.loadURL(`file://${__dirname}/index.html`);
  mainWindow.on('closed', () => (mainWindow = null));
  mainWindow.on('ready-to-show', () => {
    mainWindow.show();
  });
});
