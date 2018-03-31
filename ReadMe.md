# CaliBoy

![CaliBoyIcon](https://github.com/CheckBoxStudio/CaliBoy/blob/master/Caliboy/Resources/icon.png)

CaliBoy is a re-implementation of [Camera Calibration Toolbox for Matlab](http://www.vision.caltech.edu/bouguetj/calib_doc/) in C++ with [OpenCV](https://opencv.org/) and [Qt](https://www.qt.io/), which offers more friendly interaction.

 Principles and operation details please see our online [documentation](https://checkboxstudio.github.io/CaliBoy/).

 ## Requirements

 + [OpenCV](https://opencv.org/) (our version is 2.4.4)
 + [Qt](https://www.qt.io/) (our version is msvc2015_5.6.1)
 + [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) (embeded)
 + [QCustomPlot](http://www.qcustomplot.com/) (embedded)

 > **Note:** the Qt web viewer is embedded to see our online documentation directly in CaliBoy, however the implementation of the web viewer might vary with Qt versions. For example, Qt 5.6.1 uses class `QWebEngineView` in `WebEngineWidgets` module, while Qt 5.4.1 uses class `QWebView` in `WebKitWidgets` module.

 ---
 By [WeiQM](https://weiquanmao.github.io/) at D409.IPC.BUAA.