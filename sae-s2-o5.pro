QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/EnerSense/Admin/EnerSenseAdminWindow.cpp \
    src/EnerSense/Admin/OffersManagement/ManageTariffsDialog.cpp \
    src/EnerSense/Admin/OffersManagement/NewTariffDialog.cpp \
    src/EnerSense/Admin/OffersManagement/OfferDialog.cpp \
    src/EnerSense/Admin/OffersManagement/OffersManagementPage.cpp \
    src/EnerSense/Admin/UsersManagement/AssignOfferDialog.cpp \
    src/EnerSense/Admin/UsersManagement/CreateUserDialog.cpp \
    src/EnerSense/Admin/UsersManagement/UsersManagementPage.cpp \
    src/EnerSense/Client/EnerSenseClientWindow.cpp \
    src/EnerSense/Client/Estimate/EstimatePage.cpp \
    src/EnerSense/Client/Estimate/Invoice/InvoiceForm.cpp \
    src/EnerSense/Client/MyContract/MyContractPage.cpp \
    src/EnerSense/Client/OffersComparator/OffersComparatorPage.cpp \
    src/EnerSense/StretchTabWidget.cpp \
    src/Login/LoginDialog.cpp \
    src/main.cpp \
    src/utils/DataManager.cpp \
    src/utils/StylesHelper.cpp

HEADERS += \
    src/EnerSense/Admin/EnerSenseAdminWindow.hpp \
    src/EnerSense/Admin/OffersManagement/ManageTariffsDialog.hpp \
    src/EnerSense/Admin/OffersManagement/NewTariffDialog.hpp \
    src/EnerSense/Admin/OffersManagement/OfferDialog.hpp \
    src/EnerSense/Admin/OffersManagement/OffersManagementPage.hpp \
    src/EnerSense/Admin/UsersManagement/AssignOfferDialog.hpp \
    src/EnerSense/Admin/UsersManagement/CreateUserDialog.hpp \
    src/EnerSense/Admin/UsersManagement/UsersManagementPage.hpp \
    src/EnerSense/Client/EnerSenseClientWindow.hpp \
    src/EnerSense/Client/Estimate/EstimatePage.hpp \
    src/EnerSense/Client/Estimate/Invoice/InvoiceForm.hpp \
    src/EnerSense/Models/Contract.hpp \
    src/EnerSense/Models/Offer.hpp \
    src/EnerSense/Models/User.hpp \
    src/EnerSense/Models/Invoice.hpp \
    src/EnerSense/Client/MyContract/MyContractPage.hpp \
    src/EnerSense/Client/OffersComparator/OffersComparatorPage.hpp \
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
    assets/styles/enersenseclient.qss \
    style.qss
