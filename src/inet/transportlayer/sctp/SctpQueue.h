//
// Copyright (C) 2005-2009 Irene Ruengeler
// Copyright (C) 2009-2012 Thomas Dreibholz
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#ifndef __INET_SCTPQUEUE_H
#define __INET_SCTPQUEUE_H

#include "inet/common/INETDefs.h"
#include "inet/networklayer/common/L3Address.h"
#include "inet/transportlayer/sctp/Sctp.h"

namespace inet {
namespace sctp {

class SctpDataVariables;
class SctpAssociation;

/**
 * Abstract base class for SCTP receive queues. This class represents
 * data received by SCTP but not yet passed up to the application.
 * The class also accomodates for selective retransmission, i.e.
 * also acts as a segment buffer.
 *
 * This class goes hand-in-hand with SctpSendQueue.
 *
 * This class is polymorphic because depending on where and how you
 * use the SCTP model you might have different ideas about "sending data"
 * on a simulated connection: you might want to transmit real bytes,
 * "dummy" (byte count only), cMessage objects, etc; see discussion
 * at SctpSendQueue. Different subclasses can be written to accomodate
 * different needs.
 *
 * @see SctpSendQueue
 */
class INET_API SctpQueue : public cObject
{
  public:
    /**
     * Constructor.
     */
    SctpQueue();
    SctpQueue(SctpAssociation *assoc);

    /**
     * Virtual destructor.
     */
    ~SctpQueue();

    bool checkAndInsertChunk(const uint32_t key, SctpDataVariables *chunk);
    /* returns true if new data is inserted and false if data was present */

    SctpDataVariables *getAndExtractChunk(const uint32_t tsn);
    SctpDataVariables *extractMessage();

    void printQueue() const;

    uint32_t getQueueSize() const;

    SctpDataVariables *getFirstChunk() const;

    cMessage *getMsg(const uint32_t key) const;

    SctpDataVariables *getChunk(const uint32_t key) const;

    SctpDataVariables *getChunkFast(const uint32_t tsn, bool& firstTime);

    void removeMsg(const uint32_t key);

    bool deleteMsg(const uint32_t tsn);

    int32_t getNumBytes() const;

    SctpDataVariables *dequeueChunkBySSN(const uint16_t ssn);

    uint32_t getSizeOfFirstChunk(const L3Address& remoteAddress);

    uint16_t getFirstSsnInQueue(const uint16_t sid);

    void findEarliestOutstandingTsnsForPath(const L3Address& remoteAddress,
            uint32_t& earliestOutstandingTsn,
            uint32_t& rtxEarliestOutstandingTsn) const;

  public:
    typedef std::map<uint32_t, SctpDataVariables *> PayloadQueue;
    PayloadQueue payloadQueue;

  protected:
    SctpAssociation *assoc;    // SCTP connection object

  private:
    PayloadQueue::iterator GetChunkFastIterator;
};

} // namespace sctp
} // namespace inet

#endif

