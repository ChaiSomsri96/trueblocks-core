#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CSummary : public CBaseNode {
  public:
    blknum_t start;
    blknum_t end;
    uint64_t total;
    uint64_t count;

  public:
    CSummary(void);
    CSummary(const CSummary& su);
    virtual ~CSummary(void);
    CSummary& operator=(const CSummary& su);

    DECLARE_NODE(CSummary);

    // EXISTING_CODE
    CSummary& operator+=(const CSummary& r);
    // EXISTING_CODE
    bool operator==(const CSummary& it) const;
    bool operator!=(const CSummary& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CSummary& v1, const CSummary& v2);
    friend ostream& operator<<(ostream& os, const CSummary& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSummary& su);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSummary::CSummary(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSummary::CSummary(const CSummary& su) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(su);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSummary::~CSummary(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSummary::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSummary::initialize(void) {
    CBaseNode::initialize();

    start = 0;
    end = 0;
    total = 0;
    count = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSummary::duplicate(const CSummary& su) {
    clear();
    CBaseNode::duplicate(su);

    start = su.start;
    end = su.end;
    total = su.total;
    count = su.count;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSummary& CSummary::operator=(const CSummary& su) {
    duplicate(su);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSummary::operator==(const CSummary& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSummary& v1, const CSummary& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSummary> CSummaryArray;
extern CArchive& operator>>(CArchive& archive, CSummaryArray& array);
extern CArchive& operator<<(CArchive& archive, const CSummaryArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CSummary& sum);
extern CArchive& operator>>(CArchive& archive, CSummary& sum);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_SUMMARY;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
