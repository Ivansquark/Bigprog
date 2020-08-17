#-------------------------------------------------
#
# Project created by QtCreator 2020-01-30T11:25:40
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += serialbus
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BGN
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    cambercompensation_view.cpp \
    chambercompensation.cpp \
    electrometer_test.cpp \
        main.cpp \
        mainwindow.cpp \
    rs232.cpp \
    rs485.cpp \
        settings.cpp \
    settings_verifier.cpp \
    settings_verifier_beta.cpp \
    verifybetachambers.cpp \
    password.cpp \
    settings_verifier_gamma_ik.cpp \
    verifygammachambers.cpp \
    settings_verifier_gamma_bdg.cpp \
    settings_verifier_neutron.cpp \
    verifyneutron.cpp \
    settings_electrometer.cpp \
    settings_bdkn.cpp \
    settings_bdg.cpp \
    beta.cpp \
    beta_measuring.cpp \
    beta_verify.cpp \
    settings_verifier_gamma_nuclides.cpp \
    beta_measuring_measure.cpp \
    gamma.cpp \
    gamma_bdg.cpp \
    gamma_bdg_test.cpp \
    gamma_bdg_measuring.cpp \
    neutron.cpp \
    neutron_bdkn_test.cpp \
    neutron_measuring.cpp \
    gamma_ik.cpp \
    gamma_ik_measuring.cpp \
    gamma_bdg_verify.cpp \
    gamma_bdg_measuring_measure.cpp \
    gamma_ik_measuring_measure.cpp \
    gamma_ik_verify.cpp \
    neutron_verify.cpp \
    neutron_measuring_measure.cpp \
    emulelectrometer.cpp \
    ichamber.cpp \
    averageadc.cpp \
    electrometer_test_result.cpp \
    beta_chamber.cpp \
    auto_measurement_result.cpp \
    modbus_emulator.cpp \
    averagemodbus.cpp \
    iblock.cpp \
    block_bdg.cpp \
    abstract_measurement_measurement.cpp \
    block_bdkn.cpp \
    user_settings.cpp \
    settings_verifier_change_password.cpp \
    volthex.cpp \
    gammaik_chamber.cpp

HEADERS += \
    cambercompensation_view.h \
    chambercompensation.h \
    electrometer_test.h \
        mainwindow.h \
    rs232.h \
    rs485.h \
        settings.h \
    settings_verifier.h \
    settings_verifier_beta.h \
    verifybetachambers.h \
    password.h \
    settings_verifier_gamma_ik.h \
    verifygammachambers.h \
    settings_verifier_gamma_bdg.h \
    settings_verifier_neutron.h \
    verifyneutron.h \
    settings_electrometer.h \
    settings_bdkn.h \
    settings_bdg.h \
    beta.h \
    beta_measuring.h \
    beta_verify.h \
    settings_verifier_gamma_nuclides.h \
    beta_measuring_measure.h \
    gamma.h \
    gamma_bdg.h \
    gamma_bdg_test.h \
    gamma_bdg_measuring.h \
    neutron.h \
    neutron_bdkn_test.h \
    neutron_measuring.h \
    gamma_ik.h \
    gamma_ik_measuring.h \
    gamma_bdg_verify.h \
    gamma_bdg_measuring_measure.h \
    gamma_ik_measuring_measure.h \
    gamma_ik_verify.h \
    neutron_verify.h \
    neutron_measuring_measure.h \
    emulelectrometer.h \
    ichamber.h \
    averageadc.h \
    electrometer_test_result.h \
    beta_chamber.h \
    auto_measurement_result.h \
    modbus_emulator.h \
    averagemodbus.h \
    iblock.h \
    block_bdg.h \
    abstract_measurement_measurement.h \
    block_bdkn.h \
    user_settings.h \
    settings_verifier_change_password.h \
    volthex.h \
    gammaik_chamber.h

FORMS += \
    cambercompensation_view.ui \
    chambercompensation.ui \
    electrometer_test.ui \
        mainwindow.ui \
        settings.ui \
    settings_verifier.ui \
    settings_verifier_beta.ui \
    verifybetachambers.ui \
    settings_verifier_gamma_ik.ui \
    verifygammachambers.ui \
    settings_verifier_gamma_bdg.ui \
    settings_verifier_neutron.ui \
    verifyneutron.ui \
    settings_electrometer.ui \
    settings_bdkn.ui \
    settings_bdg.ui \
    beta.ui \
    beta_measuring.ui \
    beta_verify.ui \
    settings_verifier_gamma_nuclides.ui \
    beta_measuring_measure.ui \
    gamma.ui \
    gamma_bdg.ui \
    gamma_bdg_test.ui \
    gamma_bdg_measuring.ui \
    neutron.ui \
    neutron_bdkn_test.ui \
    neutron_measuring.ui \
    gamma_ik.ui \
    gamma_ik_measuring.ui \
    gamma_bdg_verify.ui \
    gamma_bdg_measuring_measure.ui \
    gamma_ik_measuring_measure.ui \
    gamma_ik_verify.ui \
    neutron_verify.ui \
    neutron_measuring_measure.ui \
    electrometer_test_result.ui \
    auto_measurement_result.ui \
    user_settings.ui \
    settings_verifier_change_password.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
