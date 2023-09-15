////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "LoginMopDatabase.h"

void LoginMopDatabaseConnection::DoPrepareStatements()
{
    if (!m_reconnecting)
        m_stmts.resize(MAX_LOGINMOPDATABASE_STATEMENTS);

    //PREPARE_STATEMENT(LOGINMOP_SEL_TRANSFER, "SELECT `id`, `account_ashran`, `dump` FROM transfer_ashran WHERE `realm_ashran` = ? AND state = 1", CONNECTION_SYNCH);
}
