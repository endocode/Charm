#ifndef EVENTWINDOW_H
#define EVENTWINDOW_H

#include "CharmWindow.h"

class EventView;

class EventWindow : public CharmWindow
{
    Q_OBJECT

public:
    explicit EventWindow( QWidget* parent = 0 );
    ~EventWindow();

    /* reimpl */ void stateChanged( State previous );

    // restore the view
    /* reimpl */ void restore();
public Q_SLOTS:
    /* reimpl */ void configurationChanged();

protected:
    /* reimpl */ void insertEditMenu();

Q_SIGNALS:
    /* reimpl */ void emitCommand( CharmCommand* );
    /* reimpl */ void emitCommandRollback( CharmCommand* );
    /* reimpl */ void quit();

private:
    EventView* m_eventView;

};

#endif
