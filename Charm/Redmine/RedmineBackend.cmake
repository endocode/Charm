# Add support for the Redmine backend:

# gcc specific so far:
ADD_DEFINITIONS(-std=c++11)

SET(
    CHARM_REDMINE_SOURCES
    Redmine/Backend.cpp
)
LIST( APPEND CharmApplication_SRCS ${CHARM_REDMINE_SOURCES} )
