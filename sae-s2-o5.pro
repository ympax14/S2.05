QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/EnerSense/Estimate/EstimatePage.cpp \
    src/EnerSense/Estimate/Invoice/InvoiceForm.cpp \
    src/EnerSense/EnerSenseWindow.cpp \
    src/EnerSense/MyContract/MyContractPage.cpp \
    src/EnerSense/OffersComparator/OffersComparatorPage.cpp \
    src/EnerSense/StretchTabWidget.cpp \
    src/Login/LoginDialog.cpp \
    src/main.cpp \
    src/utils/DataManager.cpp \
    src/utils/StylesHelper.cpp

HEADERS += \
    src/EnerSense/Estimate/EstimatePage.hpp \
    src/EnerSense/Estimate/Invoice/InvoiceForm.hpp \
    src/EnerSense/EnerSenseWindow.hpp \
    src/EnerSense/Models/Contract.hpp \
    src/EnerSense/Models/User.hpp \
    src/EnerSense/Models/Invoice.hpp \
    src/EnerSense/MyContract/MyContractPage.hpp \
    src/EnerSense/OffersComparator/OffersComparatorPage.hpp \
    src/EnerSense/StretchTabWidget.hpp \
    src/Login/LoginDialog.hpp \
    src/utils/DataManager.hpp \
    src/utils/StylesHelper.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ressources.qrc

DISTFILES += \
    assets/styles/enersense.qss \
    style.qss
