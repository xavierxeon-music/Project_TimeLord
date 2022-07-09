#include "SketchWidget.h"

#include "SketchModel.h"

#include <QFileDialog>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

Sketch::Widget::Widget(QWidget* widget, Model* model)
   : QDialog(widget)
   , model(model)
   , stateTree(nullptr)
{
   setWindowTitle("Sketch / Time Lord UI");
   setMinimumSize(500, 800);

   QToolBar* toolBar = new QToolBar(this);

   QAction* createAction = new QAction(QIcon(":/Add.svg"), "Create", this);
   connect(createAction, &QAction::triggered, this, &QDialog::accept);
   QToolButton* createButton = new QToolButton();
   createButton->setDefaultAction(createAction);
   createButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
   toolBar->addWidget(createButton);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/LoadFromFile.svg"), "Load From File", this, &Widget::slotLoadFromFile);
   toolBar->addAction(QIcon(":/SaveNewFile.svg"), "Save To New File", this, &Widget::slotSaveNewFile);

   QWidget* stretch = new QWidget(this);
   stretch->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
   toolBar->addWidget(stretch);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/Remove.svg"), "Reject", this, &QDialog::reject);

   stateTree = new QTreeView(this);
   stateTree->setModel(model);

   QStatusBar* statusBar = new QStatusBar(this);
   statusBar->showMessage(model->compileInfo());

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setSpacing(0);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(toolBar);
   masterLayout->addWidget(stateTree);
   masterLayout->addWidget(statusBar);
}

void Sketch::Widget::slotLoadFromFile()
{
   const QString fileName = QFileDialog::getOpenFileName(this, "Save Data", QString(), "*.timelord_sketch.json");
   if (fileName.isEmpty())
      return;

   model->loadFromFile(fileName);
}

void Sketch::Widget::slotSaveNewFile()
{
   QString fileName = QFileDialog::getSaveFileName(this, "Save Sketch", QString(), "*.json");
   if (fileName.isEmpty())
      return;

   if (!fileName.endsWith(".timelord_sketch.json"))
   {
      if (!fileName.endsWith(".json"))
         fileName += ".timelord_sketch.json";
      else
         fileName.replace(".json", ".timelord_sketch.json");
   }

   model->saveToFile(fileName);
}
