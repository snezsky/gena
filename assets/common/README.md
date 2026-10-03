# <@ project_name @>

## Build

You can build the project using your IDE of choice, or:
```bash
cmake --list-presets
cmake --preset <preset-name>
cmake --build --preset <preset-name>
ctest --preset <preset-name>
```

or configure, build and test in one step:
```bash
cmake --list-presets=workflow
cmake --workflow --preset <preset-name>
```

<% if project_type in ["QtQuickApplication", "QtWidgetsApplication"] or test_framework == "QTest" %>
## Dependencies
- [Qt 6.8+](https://www.qt.io/development/download-qt-installer-oss) 
<% endif %>
