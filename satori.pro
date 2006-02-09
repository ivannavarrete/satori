
TEMPLATE = subdirs
CONFIG += release

SUBDIRS = src/test \
          src/satori \
          src/modules/arch/avr/ui/txt/avrtxt.pro \
          src/modules/arch/avr/ui/qt/avrqt.pro \
          src/modules/comm/serial \
          src/monitor
