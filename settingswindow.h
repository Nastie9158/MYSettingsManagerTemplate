// settingswindow.h
#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QMap>
#include <QTimer>
#include <QSet>

class SettingsItem;
class QTreeWidget;
class QTreeWidgetItem;
class QStackedWidget;
class QScrollArea;
class QLineEdit;
class QLabel;

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    SettingsWindow(QWidget* parent = nullptr);
    ~SettingsWindow();

private:
    void setupUI();
    void createSettingsTree();
    void buildTreeWidget();
    void addItemToTreeWidget(SettingsItem* settingsItem, QTreeWidgetItem* parentTreeItem);
    void createPagesForGroups();
    void createPageForGroup(SettingsItem* groupItem);
    void setupConnections();
    void closeEvent(QCloseEvent* event) override;


    void setupSearchUI();
    void filterSettings(const QString& searchText);
    bool itemMatchesSearch(SettingsItem* item, const QString& searchText) const;
    void updatePageVisibility(SettingsItem* groupItem, const QString& searchText);
    void showAllItems();
    void highlightTextInLabel(QLabel* label, const QString& searchText);
    void collectAllSettingsItems();

private slots:
    void onTreeItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void onSearchTextChanged(const QString& text);
    void performSearch();

private:
    QTreeWidget* treeWidget;
    QStackedWidget* stackedWidget;
    SettingsItem* rootItem;
    QMap<SettingsItem*, QScrollArea*> groupPages;
    QMap<SettingsItem*, QTreeWidgetItem*> itemToTreeItem;

    QLineEdit* searchLineEdit;
    QLabel* searchResultsLabel;
    QTimer* searchTimer;


    QList<SettingsItem*> allSettingsItems;

    QMap<QTreeWidgetItem*, bool> originalExpansionState;

    void loadSettings();
    void saveSettings();
    void connectSignalsForAutoSave();
    QString buildSettingsPath(SettingsItem* item) const;
    void applyValueToWidget(SettingsItem* item, const QVariant& value);
};

#endif
