/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

#define FREQ 5

//-----------------------------------------------------------------------
bool COptions::handle_logs(void) {
    ENTER("handle_logs");

    ASSERT(logs && !(accounting || balances));
    ASSERT(nodeHasBalances(false));

    bool shouldDisplay = !freshen;

    CReconciliation lastStatement;
    if (apps.size() > 0 && first_record != 0)
        lastStatement.endBal = getBalanceAt(expContext().accountedFor, apps[0].blk - 1);

    bool first = true;
    blknum_t lastExportedBlock = NOPOS;
    for (size_t i = 0; i < apps.size() && (!freshen || (nProcessed < freshen_max)); i++) {
        const CAppearance_base* app = &apps[i];
        if (shouldQuit() || app->blk >= expContext().tsCnt)
            break;

        if (inRange((blknum_t)app->blk, exportRange.first, exportRange.second)) {
            CBlock block;  // do not move this from this scope
            block.blockNumber = app->blk;
            CTransaction trans;
            trans.pBlock = &block;

            string_q txFilename = getBinaryCacheFilename(CT_TXS, app->blk, app->txid);
            if (app->blk != 0 && fileExists(txFilename)) {
                // we read the data, if we find it, but....
                readTransFromBinary(trans, txFilename);
                trans.finishParse();
                trans.pBlock = &block;
                block.timestamp = trans.timestamp = (timestamp_t)expContext().tsMemMap[(app->blk * 2) + 1];

                markNeighbors(trans);
                articulateAll(trans);

                HIDE_FIELD(CFunction, "message");
                if (!isTestMode() && !(i % FREQ)) {
                    blknum_t current = first_record + nProcessed;
                    blknum_t goal = min(first_record + max_records, nTransactions);
                    ostringstream post;
                    post << " txs (max " << goal << ") for address " << allMonitors[0].address << "\r";
                    LOG_PROGRESS("Reading", current, nTransactions, post.str());
                }

            } else {
                if (app->blk == 0) {
                    address_t addr = prefundAddrMap[app->txid];
                    trans.loadTransAsPrefund(app->blk, app->txid, addr, prefundWeiMap[addr]);

                } else if (app->txid == 99997 || app->txid == 99999) {
                    trans.loadTransAsBlockReward(app->blk, app->txid, blkRewardMap[app->blk]);

                } else if (app->txid == 99998) {
                    uint64_t nUncles = getUncleCount(app->blk);
                    for (size_t u = 0; u < nUncles; u++) {
                        CBlock uncle;
                        getUncle(uncle, app->blk, u);
                        if (uncle.miner == blkRewardMap[app->blk]) {
                            trans.loadTransAsUncleReward(app->blk, uncle.blockNumber, uncle.miner);
                        }
                    }

                } else {
                    getTransaction(trans, app->blk, app->txid);
                    getFullReceipt(&trans, true);
                }

                trans.pBlock = &block;
                trans.timestamp = block.timestamp = (timestamp_t)expContext().tsMemMap[(app->blk * 2) + 1];

                markNeighbors(trans);
                articulateAll(trans);

                if (cache_txs)
                    writeTransToBinary(trans, txFilename);

                HIDE_FIELD(CFunction, "message");
                if (!isTestMode() && !(i % FREQ)) {
                    blknum_t current = first_record + nProcessed;
                    blknum_t goal = min(first_record + max_records, nTransactions);
                    ostringstream post;
                    post << " txs (max " << goal << ") for address " << allMonitors[0].address;
                    if (!isApiMode())
                        post << " " << app->blk;
                    LOG_PROGRESS("Extracting", current, nTransactions, post.str() + "\r");
                }
            }

            for (auto log : trans.receipt.logs) {
                bool showMe = true;
                if (relevant) {
                    showMe = isRelevant(log);
                    if (showMe && !emitted_by.empty())
                        showMe = wasEmittedBy(log.address);
                } else if (!emitted_by.empty()) {
                    showMe = wasEmittedBy(log.address);
                } else if (emitter) {
                    showMe = isEmitter(log.address);
                }
                nProcessed++;
                if (showMe && shouldDisplay) {
                    cout << ((isJson() && !first) ? ", " : "");
                    cout << log.Format() << endl;
                    first = false;
                }
            }
        } else if (app->blk > exportRange.second) {
            break;
        }
    }

    if (!isTestMode()) {
        if ((first_record + nProcessed) == nTransactions)
            LOG_PROGRESS((freshen ? "Finished updating" : "Finished reporting on"), (first_record + nProcessed),
                         nTransactions, " transactions for address " + allMonitors[0].address);
    }

    if (lastExportedBlock != NOPOS)
        for (auto monitor : allMonitors)
            monitor.writeLastExport(lastExportedBlock);

    reportNeighbors();

    EXIT_NOMSG(true);
}

//-----------------------------------------------------------------------
bool COptions::isEmitter(const address_t& test) const {
    for (auto monitor : allMonitors)
        if (monitor.address == test)
            return true;
    return false;
}

//-----------------------------------------------------------------------
bool COptions::wasEmittedBy(const address_t& test) const {
    for (auto e : emitted_by)
        if (e == test)
            return true;
    return false;
}

//-----------------------------------------------------------------------
bool COptions::isRelevant(const CLogEntry& log) const {
    string_q str = toLower(log.Format(STR_DISPLAY_LOGENTRY));
    for (auto monitor : allMonitors)
        if (contains(str, monitor.address.substr(2)))
            return true;
    return false;
}
