// -*- C++ -*-
//
// $Id$
#include "TransportReceiveListener.h"
#include "ReceivedDataSample.h"
#include "EntryExit.h"
#include "dds/DCPS/Util.h"

ACE_INLINE
OpenDDS::DCPS::ReceiveListenerSet::ReceiveListenerSet()
{
  DBG_ENTRY_LVL("ReceiveListenerSet","ReceiveListenerSet",5);
}


ACE_INLINE int
OpenDDS::DCPS::ReceiveListenerSet::insert(RepoId                    subscriber_id,
                                      TransportReceiveListener* listener)
{
  DBG_ENTRY_LVL("ReceiveListenerSet","insert",5);
  GuardType guard(this->lock_);
  return bind(map_, subscriber_id, listener);
}


ACE_INLINE int
OpenDDS::DCPS::ReceiveListenerSet::remove(RepoId subscriber_id)
{
  DBG_ENTRY_LVL("ReceiveListenerSet","remove",5);
  GuardType guard(this->lock_);
  if (unbind(map_, subscriber_id) != 0)
    {
      ACE_ERROR_RETURN((LM_DEBUG,
                        "(%P|%t) subscriber_id (%d) not found in map_.\n",
                        subscriber_id),
                       -1);
    }

  return 0;
}


ACE_INLINE ssize_t
OpenDDS::DCPS::ReceiveListenerSet::size() const
{
  DBG_ENTRY_LVL("ReceiveListenerSet","size",5);
  GuardType guard(this->lock_);
  return map_.size();
}


ACE_INLINE void
OpenDDS::DCPS::ReceiveListenerSet::data_received(const ReceivedDataSample& sample)
{
  DBG_ENTRY_LVL("ReceiveListenerSet","data_received",5);

  GuardType guard(this->lock_);

  char* ptr = sample.sample_->rd_ptr ();

  for (MapType::iterator itr = map_.begin();
    itr != map_.end();
    ++itr)
  {
    // reset read pointer because demarshal (in data_received()) moves it.
    sample.sample_->rd_ptr (ptr);
    itr->second->data_received(sample);
  }
}


// ACE_INLINE OpenDDS::DCPS::ReceiveListenerSet::MapType&
// OpenDDS::DCPS::ReceiveListenerSet::map()
// {
//   DBG_SUB_ENTRY("ReceiveListenerSet","map",1);
//   return this->map_;
// }


// ACE_INLINE const OpenDDS::DCPS::ReceiveListenerSet::MapType&
// OpenDDS::DCPS::ReceiveListenerSet::map() const
// {
//   DBG_SUB_ENTRY("ReceiveListenerSet","map",2);
//   return this->map_;
// }

