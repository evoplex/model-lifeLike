/**
 * Copyright (c) 2019 - Eleftheria Chatziargyriou
 * Copyright (c) 2019 - Marcos Cardinot <marcos@cardinot.net>
 *
 * This source code is licensed under the MIT license found in
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef LIFELIKE_H
#define LIFELIKE_H

#include <QBitArray>
#include <plugininterface.h>

namespace evoplex {
class LifeLike: public AbstractModel
{
public:
    bool init() override;
    bool algorithmStep() override;

private:
    int m_liveAttrId;  // the id of the 'live' node's attribute
    QBitArray m_birthLst; // the bitstream representing the neighbours needed for cell birth
    QBitArray m_survivalLst; // the bitstream representing the neighbours needed for cell survival

    QBitArray ruleToBitArray(int rule) const;
};
} // evoplex
#endif // LIFELIKE_H
