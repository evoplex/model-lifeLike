/**
 * Copyright (c) 2019 - Eleftheria Chatziargyriou
 * Copyright (c) 2019 - Marcos Cardinot <marcos@cardinot.net>
 *
 * This source code is licensed under the MIT license found in
 * the LICENSE file in the root directory of this source tree.
 */

#include "plugin.h"

namespace evoplex {

bool LifeLike::init()
{
    if (!attrExists("birth") || !attrExists("survival")) {
        qWarning() << "missing 'birth' and 'survival' attributes";
        return false;
    }
    // gets the id of the 'live' node's attribute, which is the same for all nodes
    m_liveAttrId = node(0).attrs().indexOf("live");
    // parses the ruleset
    m_birthLst = ruleToBitArray(attr("birth").toInt());
    m_survivalLst = ruleToBitArray(attr("survival").toInt());

    return m_liveAttrId >= 0 && !m_birthLst.isNull() && !m_survivalLst.isNull();
}

// convert rule to a bitstream
QBitArray LifeLike::ruleToBitArray(int rule) const
{
    QBitArray rules(9);
    if (rule == -1) {
        // particular case where all cases are false
        return rules;
    }
    int d;
    do {
        d = rule % 10;
        if (d == 9) {
            qWarning() << "The rule should not contain the digit 9.";
            return QBitArray();
        }
        if (rules[d]) {
            qWarning() << "The rule should contain only unique integers.";
            return QBitArray();
        }
        rules.setBit(d);
        rule /= 10;
    } while(rule);

    return rules;
}

bool LifeLike::algorithmStep()
{
    std::vector<Value> nextStates;
    nextStates.reserve(nodes().size());
    for (Node node : nodes()) {
        int liveNeighbours = 0;
        for (Node neighbour : node.outEdges()) {
            if (neighbour.attr(m_liveAttrId).toBool()) {
                ++liveNeighbours;
            }
        }

        if (node.attr(m_liveAttrId).toBool()) {
            // If the node is alive, then it only survives if its
            // number of neighbors is specified in the rulestring.
            // Otherwise, it dies from under/overpopulation
            nextStates.emplace_back(m_survivalLst[liveNeighbours]);
        } else {
            // Any dead cell can become alive if its number of
            // neighbors matches the one specified in the rulestring.
            // Otherwise, it remains dead.
            nextStates.emplace_back(m_birthLst[liveNeighbours]);
        }
    }
    // For each node, load the next state into the current state
    size_t i = 0;
    for (Node node : nodes()) {
        node.setAttr(m_liveAttrId, nextStates.at(i));
        ++i;
    }
    return true;
}

} // evoplex
REGISTER_PLUGIN(LifeLike)
#include "plugin.moc"
