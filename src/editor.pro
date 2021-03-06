######################################################################
# Automatically generated by qmake (2.01a) mar. mars 1 14:09:53 2011
######################################################################

TEMPLATE = app
TARGET = editor
DEPENDPATH += . \
              dependency \
              editor \
              managers \
              map \
              object_inspector \
              object_palette \
              scripting \
              tools
INCLUDEPATH += . \
               editor \
               dependency \
               managers \
               tools \
               map \
               scripting \
               object_palette \
               object_inspector \
               F:/Developpement/QScintilla-gpl-2.8.2/Qt4Qt5

QT += core gui widgets xml

LIBS += -lqscintilla2


# Input
HEADERS += constantes.hpp \
           dependency/Log.hpp \
           editor/MapEditor.hpp \
           managers/ImageManager.hpp \
           managers/ObjectManager.hpp \
           editor/Settings.hpp \
           map/EditMapAttributes.hpp \
           map/GraphicsObject.hpp \
           map/MapObjectObjectEdit.hpp \
           map/Map.hpp \
           object_inspector/ObjectInspector.hpp \
           object_inspector/ObjectInspectorTable.hpp \
           object_palette/ObjectPalette.hpp \
           object_palette/ObjectTreeWidget.hpp \
            object_palette/ObjectTreeWidgetItem.hpp \
           scripting/ScriptingArea.hpp \
           tools/FileSelector.hpp \
           tools/toolBox.hpp \
           dependency/Log.inl \
           editor/LoadingSplash.hpp \
           index.hpp \
            map/MapObject.hpp \
            map/MapArea.hpp \
            object_palette/AreaTreeWidget.hpp \
            object_palette/AreaTreeWidgetItem.hpp \
            map/MapObjectEdit.hpp \
            map/MapAreaEdit.hpp \
            map/AreasData.hpp \
            map/MiniMapView.hpp
SOURCES += main.cpp \
           dependency/Log.cpp \
           editor/MapEditor.cpp \
           managers/ImageManager.cpp \
           managers/ObjectManager.cpp \
           editor/Settings.cpp \
           map/EditMapAttributes.cpp \
           map/GraphicsObject.cpp \
           map/MapObjectEdit.cpp \
           map/Map.cpp \
           object_inspector/ObjectInspector.cpp \
           object_inspector/ObjectInspectorTable.cpp \
           object_palette/ObjectPalette.cpp \
           object_palette/ObjectTreeWidget.cpp \
            object_palette/ObjectTreeWidgetItem.cpp \
           scripting/ScriptingArea.cpp \
           tools/FileSelector.cpp \
           tools/toolBox.cpp \
           editor/LoadingSplash.cpp \
            map/MapObject.cpp \
            map/MapArea.cpp \
            object_palette/AreaTreeWidget.cpp \
            object_palette/AreaTreeWidgetItem.cpp \
            map/MapAreaEdit.cpp \
            map/AreasData.cpp \
            map/MiniMapView.cpp
RESOURCES += ressources.qrc
