// -*- C++ -*-
//
// $Id$

#include "dds/DCPS/debug.h"
#include "EntityProfiles.h"
#include "Options.h"
#include "ace/Arg_Shifter.h"
#include "ace/Log_Priority.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Configuration.h"
#include "ace/Configuration_Import_Export.h"

#if !defined (__ACE_INLINE__)
# include "Options.inl"
#endif /* ! __ACE_INLINE__ */

#include <string>
#include <iostream>

namespace { // anonymous namespace for file scope.
  //
  // Default values.
  //
  enum { DEFAULT_TEST_DURATION   =   -1};
  enum { DEFAULT_RAW_BUFFER_SIZE =  500};
  enum { DEFAULT_DOMAINID        =  521};
  enum { DEFAULT_PRIORITY        =    0};
  enum { DEFAULT_MAX             = 1450};
  enum { DEFAULT_MIN             =  800};
  enum { DEFAULT_SIZE            = 1000};
  enum { DEFAULT_DEVIATION       =  300};
  enum { DEFAULT_RATE            =  100};

  const std::string DEFAULT_RAW_OUTPUT_FILENAME = std::string();
  const OpenDDS::DCPS::DataCollector< double>::OnFull
                    DEFAULT_RAW_BUFFER_TYPE
                      = OpenDDS::DCPS::DataCollector< double>::KeepNewest;

  // Command line argument definitions.
  const char* VERBOSE_ARGUMENT             = "-v";
  const char* DURATION_ARGUMENT            = "-d";
  const char* SCENARIO_ARGUMENT            = "-f";
  const char* RAW_DATA_BUFFERSIZE_ARGUMENT = "-s";
  const char* RAW_DATA_BUFFERTYPE_ARGUMENT = "-t";
  const char* RAW_DATA_FILENAME_ARGUMENT   = "-r";

  // BUFFERTYPE argument values.
  const char* UNBOUNDED_BUFFERTYPE  = "unbounded";
  const char* KEEPNEWEST_BUFFERTYPE = "newest";
  const char* KEEPOLDEST_BUFFERTYPE = "oldest";

  // Scenario configuration file section names.
  const char* PARTICIPANT_SECTIONNAME  = "participant";
  const char* TOPIC_SECTIONNAME        = "topic";
  const char* PUBLICATION_SECTIONNAME  = "publication";
  const char* SUBSCRIPTION_SECTIONNAME = "subscription";

  // Scenario configuration file Key values.
  const char* DURATION_KEY_NAME                           = "TestDuration";
  const char* DOMAINID_KEYNAME                            = "DomainId";
  const char* USERDATA_KEYNAME                            = "UserData";
  const char* ENTITYFACTORY_KEYNAME                       = "EntityFactory";
  const char* TOPICDATA_KEYNAME                           = "TopicData";
  const char* DURABILITY_KEYNAME                          = "Durability";
  const char* DURABILITYSERVICEDURATION_KEYNAME           = "DurabilityServiceDuration";
  const char* DURABILITYSERVICEHISTORYKIND_KEYNAME        = "DurabilityServiceHistoryKind";
  const char* DURABILITYSERVICEHISTORYDEPTH_KEYNAME       = "DurabilityServiceHistoryDepth";
  const char* DURABILITYSERVICESAMPLES_KEYNAME            = "DurabilityServiceSamples";
  const char* DURABILITYSERVICEINSTANCES_KEYNAME          = "DurabilityServiceInstances";
  const char* DURABILITYSERVICESAMPLESPERINSTANCE_KEYNAME = "DurabilityServiceSamplesPerInstance";
  const char* DEADLINE_KEYNAME                            = "Deadline";
  const char* LATENCYBUDGET_KEYNAME                       = "LatencyBudget";
  const char* LIVELINESSKIND_KEYNAME                      = "LivelinessKind";
  const char* LIVELINESSDURATION_KEYNAME                  = "LivelinessDuration";
  const char* RELIABILITYKIND_KEYNAME                     = "ReliabilityKind";
  const char* RELIABILITYMAXBLOCKING_KEYNAME              = "ReliabilityMaxBlocking";
  const char* DESTINATIONORDER_KEYNAME                    = "DestinationOrder";
  const char* HISTORYKIND_KEYNAME                         = "HistoryKind";
  const char* HISTORYDEPTH_KEYNAME                        = "HistoryDepth";
  const char* RESOURCEMAXSAMPLES_KEYNAME                  = "ResourceMaxSamples";
  const char* RESOURCEMAXINSTANCES_KEYNAME                = "ResourceMaxInstances";
  const char* RESOURCEMAXSAMPLESPERINSTANCE_KEYNAME       = "ResourceMaxSamplesPerInstance";
  const char* TRANSPORTPRIORITY_KEYNAME                   = "TransportPriority";
  const char* LIFESPANDURATION_KEYNAME                    = "LifespanDuration";
  const char* OWNERSHIPKIND_KEYNAME                       = "OwnershipKind";
  const char* OWNERSHIPSTRENGTH_KEYNAME                   = "OwnershipStrength";
  const char* PRESENTATION_KEYNAME                        = "Presentation";
  const char* PRESENTATIONCOHERENT_KEYNAME                = "PresentationCoherent";
  const char* PRESENTATIONORDERED_KEYNAME                 = "PresentationOrdered";
  const char* GROUPDATA_KEYNAME                           = "GroupData";
  const char* PARTITION_KEYNAME                           = "Partition";
  const char* TIMEBASEDFILTER_KEYNAME                     = "TimeBasedFilter";
  const char* WRITERDATALIFECYCLE_KEYNAME                 = "WriterDataLifecycle";
  const char* READERDATALIFECYCLE_KEYNAME                 = "ReaderDataLifecycle";
  const char* PARTICIPANT_KEYNAME                         = "Participant";
  const char* TOPIC_KEYNAME                               = "Topic";
  const char* TRANSPORTINDEX_KEYNAME                      = "TransportIndex";
  const char* MESSAGESOURCE_KEYNAME                       = "MessageSource";
  const char* MESSAGERATE_KEYNAME                         = "MessageRate";
  const char* MESSAGESIZE_KEYNAME                         = "MessageSize";
  const char* MESSAGEMAX_KEYNAME                          = "MessageMax";
  const char* MESSAGEMIN_KEYNAME                          = "MessageMin";
  const char* MESSAGEDEVIATION_KEYNAME                    = "MessageDeviation";
  const char* DATACOLLECTIONFILE_KEYNAME                  = "DataCollectionFile";
  const char* DATACOLLECTIONBOUND_KEYNAME                 = "DataCollectionBound";
  const char* DATACOLLECTIONRETENTION_KEYNAME             = "DataCollectionRetention";

} // end of anonymous namespace.

namespace Test {

Options::~Options()
{
  for( ParticipantProfileMap::iterator current = this->participantProfileMap_.begin();
       current != this->participantProfileMap_.end();
       ++current
     ) {
    delete current->second;
  }
  this->participantProfileMap_.clear();

  for( TopicProfileMap::iterator current = this->topicProfileMap_.begin();
       current != this->topicProfileMap_.end();
       ++current
     ) {
    delete current->second;
  }
  this->topicProfileMap_.clear();

  for( PublicationProfileMap::iterator current = this->publicationProfileMap_.begin();
       current != this->publicationProfileMap_.end();
       ++current
     ) {
    delete current->second;
  }
  this->publicationProfileMap_.clear();

  for( SubscriptionProfileMap::iterator current = this->subscriptionProfileMap_.begin();
       current != this->subscriptionProfileMap_.end();
       ++current
     ) {
    delete current->second;
  }
  this->subscriptionProfileMap_.clear();
}

Options::Options( int argc, char** argv, char** /* envp */)
 : verbose_(           false),
   configured_(        true),
   duration_(          DEFAULT_TEST_DURATION),
   rawOutputFilename_( DEFAULT_RAW_OUTPUT_FILENAME),
   rawBufferSize_(   DEFAULT_RAW_BUFFER_SIZE),
   rawBufferType_(   DEFAULT_RAW_BUFFER_TYPE)
{
  ACE_Arg_Shifter parser( argc, argv);
  while( parser.is_anything_left()) {
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::Options() - ")
        ACE_TEXT("processing argument: %s.\n"),
        parser.get_current()
      ));
    }
    const char* currentArg = 0;
    if( 0 != (currentArg = parser.get_the_parameter( DURATION_ARGUMENT))) {
      this->duration_ = ACE_OS::atoi( currentArg);
      parser.consume_arg();

    } else if( 0 != (currentArg = parser.get_the_parameter( SCENARIO_ARGUMENT))) {
      this->configureScenarios( currentArg);
      parser.consume_arg();

    } else if( 0 != (currentArg = parser.get_the_parameter( RAW_DATA_BUFFERSIZE_ARGUMENT))) {
      this->rawBufferSize_ = ACE_OS::atoi( currentArg);
      parser.consume_arg();

    } else if( 0 != (currentArg = parser.get_the_parameter( RAW_DATA_BUFFERTYPE_ARGUMENT))) {
      std::string bufferType = currentArg;
      if( bufferType == UNBOUNDED_BUFFERTYPE) {
        this->rawBufferType_ = ::OpenDDS::DCPS::DataCollector<double>::Unbounded;

      } else if( bufferType == KEEPNEWEST_BUFFERTYPE) {
        this->rawBufferType_ = ::OpenDDS::DCPS::DataCollector<double>::KeepNewest;

      } else if( bufferType == KEEPOLDEST_BUFFERTYPE) {
        this->rawBufferType_ = ::OpenDDS::DCPS::DataCollector<double>::KeepOldest;

      } else {
        if( this->verbose()) {
          ACE_DEBUG((LM_DEBUG,
            ACE_TEXT("(%P|%t) Options::Options() - ")
            ACE_TEXT("unrecognized value for %s argument: %s, using default.\n"),
            RAW_DATA_BUFFERTYPE_ARGUMENT,
            currentArg
          ));
        }
      }
      parser.consume_arg();

    } else if( 0 != (currentArg = parser.get_the_parameter( RAW_DATA_FILENAME_ARGUMENT))) {
      this->rawOutputFilename_ = currentArg;
      if( this->verbose()) {
        ACE_DEBUG((LM_DEBUG,
          ACE_TEXT("(%P|%t) Options::Options() - ")
          ACE_TEXT("Setting raw latency data output file to %s.\n"),
          currentArg
        ));
      }
      parser.consume_arg();

    } else if( 0 <= (parser.cur_arg_strncasecmp( VERBOSE_ARGUMENT))) {
      this->verbose_ = true;
      if( this->verbose()) {
        ACE_DEBUG((LM_DEBUG,
          ACE_TEXT("(%P|%t) Options::Options() - ")
          ACE_TEXT("Setting VERBOSE mode.\n")
        ));
      }
      parser.consume_arg();

    } else {
      parser.ignore_arg();
    }
  }
}

void
Options::configureScenarios( const char* filename)
{
  if( this->verbose()) {
    ACE_DEBUG((LM_DEBUG,
      ACE_TEXT("(%P|%t) Options::configureScenarios() - ")
      ACE_TEXT("configuring using file: %s.\n"),
      filename
    ));
  }

  ACE_Configuration_Heap heap;
  if( 0 != heap.open()) {
    ACE_ERROR((LM_ERROR,
      ACE_TEXT("(%P|%t) ERROR: Options::configureScenarios() - ")
      ACE_TEXT("failed to open() configuration heap.\n")
    ));
    return;
  }

  ACE_Ini_ImpExp import( heap);
  if( 0 != import.import_config( filename)) {
    ACE_ERROR((LM_ERROR,
      ACE_TEXT("(%P|%t) ERROR: Options::configureScenarios() - ")
      ACE_TEXT("failed to import configuration file.\n")
    ));
    return;
  }

  // Process common (no section) data here.
  const ACE_Configuration_Section_Key& root = heap.root_section();

  // TestDuration = <seconds>                   OPTIONAL
  ACE_TString durationString;
  if( 0 == heap.get_string_value( root, DURATION_KEY_NAME, durationString)) {
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::configureScenarios() - ")
        ACE_TEXT("setting test duration to: %d.\n"),
        durationString.c_str()
      ));
    }
    this->duration_ = ACE_OS::atoi( durationString.c_str());
  }

  // Read and process the DDS Entity specification sections.
  this->configureEntities( heap);
}

void
Options::configureEntities( ACE_Configuration_Heap& heap)
{
  // Find all of the [participant] sections.
  ACE_Configuration_Section_Key participantKey;
  if( 0 == heap.open_section( heap.root_section(), PARTICIPANT_SECTIONNAME, 0, participantKey)) {
    // Process each [participant] subsection.
    ACE_TString sectionName;
    for( int index = 0;
         (0 == heap.enumerate_sections( participantKey, index, sectionName));
         ++index
       ) {
      if( this->verbose()) {
        ACE_DEBUG((LM_DEBUG,
          ACE_TEXT("(%P|%t) Options::configureEntities() - ")
          ACE_TEXT("configuring participant %s.\n"),
          sectionName.c_str()
        ));
      }

      ACE_Configuration_Section_Key sectionKey;
      if( 0 == heap.open_section( participantKey, sectionName.c_str(), 0, sectionKey)) {
        loadParticipant( heap, sectionKey, sectionName.c_str());

      } else {
        ACE_ERROR((LM_ERROR,
          ACE_TEXT("(%P|%t) ERROR: Options::configureEntities() - ")
          ACE_TEXT("unable to open section %s, skipping.\n"),
          sectionName.c_str()
        ));
        this->configured_ = false;
      }
    }

  } else {
    ACE_DEBUG((LM_DEBUG,
      ACE_TEXT("(%P|%t) Options::configureEntities() - ")
      ACE_TEXT("failed to find any participant definitions in ")
      ACE_TEXT("scenario definition file.\n")
    ));
    this->configured_ = false;
  }

  // Find all of the [topic] sections.
  ACE_Configuration_Section_Key topicKey;
  if( 0 == heap.open_section( heap.root_section(), TOPIC_SECTIONNAME, 0, topicKey)) {
    // Process each [topic] subsection.
    ACE_TString sectionName;
    for( int index = 0;
         (0 == heap.enumerate_sections( topicKey, index, sectionName));
         ++index
       ) {
      if( this->verbose()) {
        ACE_DEBUG((LM_DEBUG,
          ACE_TEXT("(%P|%t) Options::configureEntities() - ")
          ACE_TEXT("configuring topic %s.\n"),
          sectionName.c_str()
        ));
      }

      ACE_Configuration_Section_Key sectionKey;
      if( 0 == heap.open_section( topicKey, sectionName.c_str(), 0, sectionKey)) {
        loadTopic( heap, sectionKey, sectionName.c_str());

      } else {
        ACE_ERROR((LM_ERROR,
          ACE_TEXT("(%P|%t) ERROR: Options::configureEntities() - ")
          ACE_TEXT("unable to open section %s, skipping.\n"),
          sectionName.c_str()
        ));
        this->configured_ = false;
      }
    }

  } else {
    ACE_DEBUG((LM_DEBUG,
      ACE_TEXT("(%P|%t) Options::configureEntities() - ")
      ACE_TEXT("failed to find any topic definitions in ")
      ACE_TEXT("scenario definition file.\n")
    ));
    this->configured_ = false;
  }

  // Find all of the [publication] sections.
  ACE_Configuration_Section_Key publicationKey;
  if( 0 == heap.open_section( heap.root_section(), PUBLICATION_SECTIONNAME, 0, publicationKey)) {
    // Process each [publication] subsection.
    ACE_TString sectionName;
    for( int index = 0;
         (0 == heap.enumerate_sections( publicationKey, index, sectionName));
         ++index
       ) {
      if( this->verbose()) {
        ACE_DEBUG((LM_DEBUG,
          ACE_TEXT("(%P|%t) Options::configureEntities() - ")
          ACE_TEXT("configuring publication %s.\n"),
          sectionName.c_str()
        ));
      }

      ACE_Configuration_Section_Key sectionKey;
      if( 0 == heap.open_section( publicationKey, sectionName.c_str(), 0, sectionKey)) {
        loadPublication( heap, sectionKey, sectionName.c_str());

      } else {
        ACE_ERROR((LM_ERROR,
          ACE_TEXT("(%P|%t) ERROR: Options::configureEntities() - ")
          ACE_TEXT("unable to open section %s, skipping.\n"),
          sectionName.c_str()
        ));
        this->configured_ = false;
      }
    }

  } else {
    ACE_DEBUG((LM_DEBUG,
      ACE_TEXT("(%P|%t) Options::configureEntities() - ")
      ACE_TEXT("failed to find any publication definitions in ")
      ACE_TEXT("scenario definition file.\n")
    ));
    this->configured_ = false;
  }

  // Find all of the [subscription] sections.
  ACE_Configuration_Section_Key subscriptionKey;
  if( 0 == heap.open_section( heap.root_section(), SUBSCRIPTION_SECTIONNAME, 0, subscriptionKey)) {
    // Process each [subscription] subsection.
    ACE_TString sectionName;
    for( int index = 0;
         (0 == heap.enumerate_sections( subscriptionKey, index, sectionName));
         ++index
       ) {
      if( this->verbose()) {
        ACE_DEBUG((LM_DEBUG,
          ACE_TEXT("(%P|%t) Options::configureEntities() - ")
          ACE_TEXT("configuring subscription %s.\n"),
          sectionName.c_str()
        ));
      }

      ACE_Configuration_Section_Key sectionKey;
      if( 0 == heap.open_section( subscriptionKey, sectionName.c_str(), 0, sectionKey)) {
        loadSubscription( heap, sectionKey, sectionName.c_str());

      } else {
        ACE_ERROR((LM_ERROR,
          ACE_TEXT("(%P|%t) ERROR: Options::configureEntities() - ")
          ACE_TEXT("unable to open section %s, skipping.\n"),
          sectionName.c_str()
        ));
      }
    }

  } else {
    ACE_DEBUG((LM_DEBUG,
      ACE_TEXT("(%P|%t) Options::configureEntities() - ")
      ACE_TEXT("failed to find any subscription definitions in ")
      ACE_TEXT("scenario definition file.\n")
    ));
    this->configured_ = false;
  }

}

void
Options::loadParticipant(
  ACE_Configuration_Heap& heap,
  ACE_Configuration_Section_Key& sectionKey,
  std::string sectionName
)
{
  /**
   * [participant/<name>]
   *   # Participant Qos Policy values
   *   UserData      = <string>
   *   EntityFactory = <bool>
   *   # Test execution parameters
   *   DomainId      = <number>
   */

  // Note that this requires that the Service Participant already be
  // initialized before we configure from the scenario file.  Also,
  // since we have not created any Entities yet, we go to the initial
  // default values rather than to the containing Entity.
  ParticipantProfile* profile = new ParticipantProfile();
  profile->qos = TheServiceParticipant->initial_DomainParticipantQos();
  ACE_TString valueString;

  // DomainId = <number>
  profile->domainId = DEFAULT_DOMAINID;
  if( 0 == heap.get_string_value( sectionKey, DOMAINID_KEYNAME, valueString)) {
    profile->domainId = ACE_OS::atoi( valueString.c_str());
  }
  if( this->verbose()) {
    ACE_DEBUG((LM_DEBUG,
      ACE_TEXT("(%P|%t) Options::loadParticipant() - ")
      ACE_TEXT("  [participant/%s] %s == %d.\n"),
      sectionName.c_str(),
      DOMAINID_KEYNAME,
      profile->domainId
    ));
  }

  // UserData      = <string>     OPTIONAL
  valueString.clear();
  heap.get_string_value( sectionKey, USERDATA_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.user_data.value.length( valueString.length());
    profile->qos.user_data.value.replace(
      valueString.length(),
      valueString.length(),
      const_cast<CORBA::Octet*>(
        reinterpret_cast<const CORBA::Octet*>( valueString.c_str())
      )
    );
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadParticipant() - ")
        ACE_TEXT("  [participant/%s] %s == %s.\n"),
        sectionName.c_str(),
        USERDATA_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // EntityFactory = <bool>       OPTIONAL
  valueString.clear();
  if( 0 == heap.get_string_value( sectionKey, ENTITYFACTORY_KEYNAME, valueString)) {
    profile->qos.entity_factory.autoenable_created_entities
      = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadParticipant() - ")
        ACE_TEXT("  [participant/%s] %s == %d.\n"),
        sectionName.c_str(),
        ENTITYFACTORY_KEYNAME,
        profile->qos.entity_factory.autoenable_created_entities
      ));
    }
  }

  // Store the profile for the current participant.
  this->participantProfileMap_[ sectionName] = profile;
}

void
Options::loadTopic(
  ACE_Configuration_Heap& heap,
  ACE_Configuration_Section_Key& sectionKey,
  std::string sectionName
)
{
  /**
   * [topic/<name>]
   *   # Topic Qos Policy values
   *   TopicData                           = <string>
   *   Durability                          = <string> # One of VOLATILE, LOCAL, TRANSIENT, PERSISTENT
   *   DurabilityServiceDuration           = <number>
   *   DurabilityServiceHistoryKind        = <string> # One of ALL, LAST
   *   DurabilityServiceHistoryDepth       = <number>
   *   DurabilityServiceSamples            = <number>
   *   DurabilityServiceInstances          = <number>
   *   DurabilityServiceSamplesPerInstance = <number>
   *   Deadline                            = <number>
   *   LatencyBudget                       = <number>
   *   LivelinessKind                      = <string> # One of AUTOMATIC, PARTICIPANT, TOPIC
   *   LivelinessDuration                  = <number>
   *   ReliabilityKind                     = <string> # One of BEST_EFFORT, RELIABLE
   *   ReliabilityMaxBlocking              = <number>
   *   DestinationOrder                    = <string> # One of SOURCE, RECEPTION
   *   HistoryKind                         = <string> # One of ALL, LAST
   *   HistoryDepth                        = <number>
   *   ResourceMaxSamples                  = <number>
   *   ResourceMaxInstances                = <number>
   *   ResourceMaxSamplesPerInstance       = <number>
   *   TransportPriority                   = <number>
   *   LifespanDuration                    = <number>
   *   OwnershipKind                       = <string> # One of SHARED, EXCLUSIVE
   *   # Test execution parameters
   *   Participant                         = <string> # One of participant <name>
   */

  // Note that this requires that the Service Participant already be
  // initialized before we configure from the scenario file.  Also,
  // since we have not created any Entities yet, we go to the initial
  // default values rather than to the containing Entity.
  TopicProfile* profile = new TopicProfile();
  profile->qos = TheServiceParticipant->initial_TopicQos();
  ACE_TString valueString;

  // TopicData                           = <string>
  heap.get_string_value( sectionKey, TOPICDATA_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.topic_data.value.length( valueString.length());
    profile->qos.topic_data.value.replace(
      valueString.length(),
      valueString.length(),
      const_cast<CORBA::Octet*>(
        reinterpret_cast<const CORBA::Octet*>( valueString.c_str())
      )
    );
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %s.\n"),
        sectionName.c_str(),
        TOPICDATA_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // Durability = <string> # One of VOLATILE, LOCAL, TRANSIENT, PERSISTENT
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITY_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "VOLATILE") {
      profile->qos.durability.kind = ::DDS::VOLATILE_DURABILITY_QOS;

    } else if( valueString == "LOCAL") {
      profile->qos.durability.kind = ::DDS::TRANSIENT_LOCAL_DURABILITY_QOS;

    } else if( valueString == "TRANSIENT") {
      profile->qos.durability.kind = ::DDS::TRANSIENT_DURABILITY_QOS;

    } else if( valueString == "PERSISTENT") {
      profile->qos.durability.kind = ::DDS::PERSISTENT_DURABILITY_QOS;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadTopic() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        DURABILITY_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %s.\n"),
        sectionName.c_str(),
        DURABILITY_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // DurabilityServiceDuration           = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITYSERVICEDURATION_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.durability_service.service_cleanup_delay.nanosec = 0;
    profile->qos.durability_service.service_cleanup_delay.sec
      = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %d.\n"),
        sectionName.c_str(),
        DURABILITYSERVICEDURATION_KEYNAME,
        profile->qos.durability_service.service_cleanup_delay.sec
      ));
    }
  }

  // DurabilityServiceHistoryKind        = <string> # One of ALL, LAST
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITYSERVICEHISTORYKIND_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "ALL") {
      profile->qos.durability_service.history_kind = ::DDS::KEEP_ALL_HISTORY_QOS;

    } else if( valueString == "LAST") {
      profile->qos.durability_service.history_kind = ::DDS::KEEP_LAST_HISTORY_QOS;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadTopic() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        DURABILITYSERVICEHISTORYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %s.\n"),
        sectionName.c_str(),
        DURABILITYSERVICEHISTORYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // DurabilityServiceHistoryDepth       = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITYSERVICEHISTORYDEPTH_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.durability_service.history_depth = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %d.\n"),
        sectionName.c_str(),
        DURABILITYSERVICEHISTORYDEPTH_KEYNAME,
        profile->qos.durability_service.history_depth
      ));
    }
  }

  // DurabilityServiceSamples            = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITYSERVICESAMPLES_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.durability_service.max_samples = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %d.\n"),
        sectionName.c_str(),
        DURABILITYSERVICESAMPLES_KEYNAME,
        profile->qos.durability_service.max_samples
      ));
    }
  }

  // DurabilityServiceInstances          = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITYSERVICEINSTANCES_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.durability_service.max_instances = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %d.\n"),
        sectionName.c_str(),
        DURABILITYSERVICEINSTANCES_KEYNAME,
        profile->qos.durability_service.max_instances
      ));
    }
  }

  // DurabilityServiceSamplesPerInstance = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITYSERVICESAMPLESPERINSTANCE_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.durability_service.max_samples_per_instance
      = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %s.\n"),
        sectionName.c_str(),
        DURABILITYSERVICESAMPLESPERINSTANCE_KEYNAME,
        profile->qos.durability_service.max_samples_per_instance
      ));
    }
  }

  // Deadline                            = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, DEADLINE_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.deadline.period.nanosec = 0;
    profile->qos.deadline.period.sec = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %d.\n"),
        sectionName.c_str(),
        DEADLINE_KEYNAME,
        profile->qos.deadline.period.sec
      ));
    }
  }

  // LatencyBudget                       = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, LATENCYBUDGET_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.latency_budget.duration.nanosec = 0;
    profile->qos.latency_budget.duration.sec = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %d.\n"),
        sectionName.c_str(),
        LATENCYBUDGET_KEYNAME,
        profile->qos.latency_budget.duration.sec
      ));
    }
  }

  // LivelinessKind                      = <string> # One of AUTOMATIC, PARTICIPANT, TOPIC
  valueString.clear();
  heap.get_string_value( sectionKey, LIVELINESSKIND_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "AUTOMATIC") {
      profile->qos.liveliness.kind = ::DDS::AUTOMATIC_LIVELINESS_QOS;

    } else if( valueString == "PARTICIPANT") {
      profile->qos.liveliness.kind = ::DDS::MANUAL_BY_PARTICIPANT_LIVELINESS_QOS;

    } else if( valueString == "TOPIC") {
      profile->qos.liveliness.kind = ::DDS::MANUAL_BY_TOPIC_LIVELINESS_QOS;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadTopic() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        LIVELINESSKIND_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %s.\n"),
        sectionName.c_str(),
        LIVELINESSKIND_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // LivelinessDuration                  = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, LIVELINESSDURATION_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.liveliness.lease_duration.nanosec = 0;
    profile->qos.liveliness.lease_duration.sec = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %d.\n"),
        sectionName.c_str(),
        LIVELINESSDURATION_KEYNAME,
        profile->qos.liveliness.lease_duration.sec
      ));
    }
  }

  // ReliabilityKind                     = <string> # One of BEST_EFFORT, RELIABLE
  valueString.clear();
  heap.get_string_value( sectionKey, RELIABILITYKIND_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "BEST_EFFORT") {
      profile->qos.reliability.kind = ::DDS::BEST_EFFORT_RELIABILITY_QOS;

    } else if( valueString == "RELIABLE") {
      profile->qos.reliability.kind = ::DDS::RELIABLE_RELIABILITY_QOS;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadTopic() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        RELIABILITYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %s.\n"),
        sectionName.c_str(),
        RELIABILITYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // ReliabilityMaxBlocking              = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, RELIABILITYMAXBLOCKING_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.reliability.max_blocking_time.nanosec = 0;
    profile->qos.reliability.max_blocking_time.sec = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %d.\n"),
        sectionName.c_str(),
        RELIABILITYMAXBLOCKING_KEYNAME,
        profile->qos.reliability.max_blocking_time.sec
      ));
    }
  }

  // DestinationOrder                    = <string> # One of SOURCE, RECEPTION
  valueString.clear();
  heap.get_string_value( sectionKey, DESTINATIONORDER_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "SOURCE") {
      profile->qos.destination_order.kind = ::DDS::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;

    } else if( valueString == "RECEPTION") {
      profile->qos.destination_order.kind = ::DDS::BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadTopic() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        DESTINATIONORDER_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %s.\n"),
        sectionName.c_str(),
        DESTINATIONORDER_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // HistoryKind                         = <string> # One of ALL, LAST
  valueString.clear();
  heap.get_string_value( sectionKey, HISTORYKIND_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "ALL") {
      profile->qos.history.kind = ::DDS::KEEP_ALL_HISTORY_QOS;

    } else if( valueString == "LAST") {
      profile->qos.history.kind = ::DDS::KEEP_LAST_HISTORY_QOS;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadTopic() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        HISTORYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %s.\n"),
        sectionName.c_str(),
        HISTORYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // HistoryDepth                        = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, HISTORYDEPTH_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.history.depth = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %d.\n"),
        sectionName.c_str(),
        HISTORYDEPTH_KEYNAME,
        profile->qos.history.depth
      ));
    }
  }

  // ResourceMaxSamples                  = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, RESOURCEMAXSAMPLES_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.resource_limits.max_samples = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %d.\n"),
        sectionName.c_str(),
        RESOURCEMAXSAMPLES_KEYNAME,
        profile->qos.resource_limits.max_samples
      ));
    }
  }

  // ResourceMaxInstances                = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, RESOURCEMAXINSTANCES_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.resource_limits.max_instances = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %d.\n"),
        sectionName.c_str(),
        RESOURCEMAXINSTANCES_KEYNAME,
        profile->qos.resource_limits.max_instances
      ));
    }
  }

  // ResourceMaxSamplesPerInstance       = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, RESOURCEMAXSAMPLESPERINSTANCE_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.resource_limits.max_samples_per_instance = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %d.\n"),
        sectionName.c_str(),
        RESOURCEMAXSAMPLESPERINSTANCE_KEYNAME,
        profile->qos.resource_limits.max_samples_per_instance
      ));
    }
  }

  // TransportPriority                   = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, TRANSPORTPRIORITY_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.transport_priority.value = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %d.\n"),
        sectionName.c_str(),
        TRANSPORTPRIORITY_KEYNAME,
        profile->qos.transport_priority.value
      ));
    }
  }

  // LifespanDuration                    = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, LIFESPANDURATION_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->qos.lifespan.duration.nanosec = 0;
    profile->qos.lifespan.duration.sec = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %d.\n"),
        sectionName.c_str(),
        LIFESPANDURATION_KEYNAME,
        profile->qos.lifespan.duration.sec
      ));
    }
  }

  // OwnershipKind                       = <string> # One of SHARED, EXCLUSIVE
  valueString.clear();
  heap.get_string_value( sectionKey, OWNERSHIPKIND_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "SHARED") {
      profile->qos.ownership.kind = ::DDS::SHARED_OWNERSHIP_QOS;

    } else if( valueString == "EXCLUSIVE") {
      profile->qos.ownership.kind = ::DDS::EXCLUSIVE_OWNERSHIP_QOS;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadTopic() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        OWNERSHIPKIND_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %s.\n"),
        sectionName.c_str(),
        OWNERSHIPKIND_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // Participant      = <string> # One of participant <name>
  valueString.clear();
  heap.get_string_value( sectionKey, PARTICIPANT_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->participant = valueString.c_str();
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadTopic() - ")
        ACE_TEXT("  [topic/%s] %s == %s.\n"),
        sectionName.c_str(),
        PARTICIPANT_KEYNAME,
        profile->participant.c_str()
      ));
    }
  }

  // Store the profile for the current participant.
  this->topicProfileMap_[ sectionName] = profile;
}

void
Options::loadPublication(
  ACE_Configuration_Heap& heap,
  ACE_Configuration_Section_Key& sectionKey,
  std::string sectionName
)
{
  /**
   * [publication/<name>]
   *   # Publisher Qos Policy values
   *   Presentation                        = <string> # One of INSTANCE, TOPIC, GROUP
   *   PresentationCoherent                = <bool> # Boolean: numeric 0 or 1
   *   PresentationOrdered                 = <bool> # Boolean: numeric 0 or 1
   *   Partition                           = <string> # Only single value supported
   *   GroupData                           = <string>
   *   EntityFactory                       = <bool> # Boolean: numeric 0 or 1
   *   # DataWriter Qos Policy values
   *   Durability                          = <string> # One of VOLATILE, LOCAL, TRANSIENT, PERSISTENT
   *   DurabilityServiceDuration           = <number>
   *   DurabilityServiceHistoryKind        = <string> # One of ALL, LAST
   *   DurabilityServiceHistoryDepth       = <number>
   *   DurabilityServiceSamples            = <number>
   *   DurabilityServiceInstances          = <number>
   *   DurabilityServiceSamplesPerInstance = <number>
   *   Deadline                            = <number>
   *   LatencyBudget                       = <number>
   *   LivelinessKind                      = <string> # One of AUTOMATIC, PARTICIPANT, TOPIC
   *   LivelinessDuration                  = <number>
   *   ReliabilityKind                     = <string> # One of BEST_EFFORT, RELIABLE
   *   ReliabilityMaxBlocking              = <number>
   *   DestinationOrder                    = <string> # One of SOURCE, RECEPTION
   *   HistoryKind                         = <string> # One of ALL, LAST
   *   HistoryDepth                        = <number>
   *   ResourceMaxSamples                  = <number>
   *   ResourceMaxInstances                = <number>
   *   ResourceMaxSamplesPerInstance       = <number>
   *   TransportPriority                   = <number>
   *   Lifespan                            = <number>
   *   UserData                            = <string>
   *   #VERSION1.2# OwnershipKind                       = <string> # One of SHARED, EXCLUSIVE
   *   OwnershipStrength                   = <number>
   *   WriterDataLifecycle                 = <bool> # Boolean: numeric 0 or 1
   *   # Test execution parameters
   *   Topic            = <string> # One of topic <name>
   *   TransportIndex   = <number> # Index into transport configurations
   *   MessageSource    = <string> # One of subscription <name>
   *   MessageRate      = <number> # Samples per second
   *   MessageSize      = <number> # bytes per sample
   *   MessageMax       = <number> # upper bound for size
   *   MessageMin       = <number> # lower bound for size
   *   MessageDeviation = <number> # standard deviation for size
   */

  // Note that this requires that the Service Participant already be
  // initialized before we configure from the scenario file.  Also,
  // since we have not created any Entities yet, we go to the initial
  // default values rather than to the containing Entity.
  PublicationProfile* profile = new PublicationProfile();
  profile->publisherQos  = TheServiceParticipant->initial_PublisherQos();
  profile->writerQos     = TheServiceParticipant->initial_DataWriterQos();
  profile->writerQosMask = 0;
  ACE_TString valueString;

  // Presentation                        = <string> # One of INSTANCE, TOPIC, GROUP
  valueString.clear();
  heap.get_string_value( sectionKey, PRESENTATION_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "INSTANCE") {
      profile->publisherQos.presentation.access_scope = ::DDS::INSTANCE_PRESENTATION_QOS;

    } else if( valueString == "TOPIC") {
      profile->publisherQos.presentation.access_scope = ::DDS::TOPIC_PRESENTATION_QOS;

    } else if( valueString == "GROUP") {
      profile->publisherQos.presentation.access_scope = ::DDS::GROUP_PRESENTATION_QOS;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadPublication() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        PRESENTATION_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %s.\n"),
        sectionName.c_str(),
        PRESENTATION_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // PresentationCoherent                = <bool> # Boolean: numeric 0 or 1
  valueString.clear();
  if( 0 == heap.get_string_value( sectionKey, PRESENTATIONCOHERENT_KEYNAME, valueString)) {
    profile->publisherQos.presentation.coherent_access
      = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        PRESENTATIONCOHERENT_KEYNAME,
        profile->publisherQos.presentation.coherent_access
      ));
    }
  }

  // PresentationOrdered                 = <bool> # Boolean: numeric 0 or 1
  valueString.clear();
  if( 0 == heap.get_string_value( sectionKey, PRESENTATIONORDERED_KEYNAME, valueString)) {
    profile->publisherQos.presentation.ordered_access
      = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        PRESENTATIONORDERED_KEYNAME,
        profile->publisherQos.presentation.ordered_access
      ));
    }
  }

  // Partition                           = <string> # Only single value supported
  valueString.clear();
  heap.get_string_value( sectionKey, PARTITION_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->publisherQos.partition.name.length( 1);
    profile->publisherQos.partition.name[0] = ACE_OS::strdup( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %s.\n"),
        sectionName.c_str(),
        PARTITION_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // GroupData                           = <string>
  valueString.clear();
  heap.get_string_value( sectionKey, GROUPDATA_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->publisherQos.group_data.value.length( valueString.length());
    profile->publisherQos.group_data.value.replace(
      valueString.length(),
      valueString.length(),
      const_cast<CORBA::Octet*>(
        reinterpret_cast<const CORBA::Octet*>( valueString.c_str())
      )
    );
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %s.\n"),
        sectionName.c_str(),
        GROUPDATA_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // EntityFactory                       = <bool> # Boolean: numeric 0 or 1
  valueString.clear();
  if( 0 == heap.get_string_value( sectionKey, ENTITYFACTORY_KEYNAME, valueString)) {
    profile->publisherQos.entity_factory.autoenable_created_entities
      = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        ENTITYFACTORY_KEYNAME,
        profile->publisherQos.entity_factory.autoenable_created_entities
      ));
    }
  }

  // Durability = <string> # One of VOLATILE, LOCAL, TRANSIENT, PERSISTENT
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITY_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "VOLATILE") {
      profile->writerQos.durability.kind = ::DDS::VOLATILE_DURABILITY_QOS;
      profile->writerQosMask |= SetDurabilityQos;

    } else if( valueString == "LOCAL") {
      profile->writerQos.durability.kind = ::DDS::TRANSIENT_LOCAL_DURABILITY_QOS;
      profile->writerQosMask |= SetDurabilityQos;

    } else if( valueString == "TRANSIENT") {
      profile->writerQos.durability.kind = ::DDS::TRANSIENT_DURABILITY_QOS;
      profile->writerQosMask |= SetDurabilityQos;

    } else if( valueString == "PERSISTENT") {
      profile->writerQos.durability.kind = ::DDS::PERSISTENT_DURABILITY_QOS;
      profile->writerQosMask |= SetDurabilityQos;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadPublication() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        DURABILITY_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %s.\n"),
        sectionName.c_str(),
        DURABILITY_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // DurabilityServiceDuration           = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITYSERVICEDURATION_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.durability_service.service_cleanup_delay.nanosec = 0;
    profile->writerQos.durability_service.service_cleanup_delay.sec
      = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetDurabilityServiceDurationQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        DURABILITYSERVICEDURATION_KEYNAME,
        profile->writerQos.durability_service.service_cleanup_delay.sec
      ));
    }
  }

  // DurabilityServiceHistoryKind        = <string> # One of ALL, LAST
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITYSERVICEHISTORYKIND_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "ALL") {
      profile->writerQos.durability_service.history_kind = ::DDS::KEEP_ALL_HISTORY_QOS;
      profile->writerQosMask |= SetDurabilityServiceHistoryKindQos;

    } else if( valueString == "LAST") {
      profile->writerQos.durability_service.history_kind = ::DDS::KEEP_LAST_HISTORY_QOS;
      profile->writerQosMask |= SetDurabilityServiceHistoryKindQos;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadPublication() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        DURABILITYSERVICEHISTORYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %s.\n"),
        sectionName.c_str(),
        DURABILITYSERVICEHISTORYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // DurabilityServiceHistoryDepth       = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITYSERVICEHISTORYDEPTH_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.durability_service.history_depth = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetDurabilityServiceHistoryDepthQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        DURABILITYSERVICEHISTORYDEPTH_KEYNAME,
        profile->writerQos.durability_service.history_depth
      ));
    }
  }

  // DurabilityServiceSamples            = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITYSERVICESAMPLES_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.durability_service.max_samples = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetDurabilityServiceSamplesQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        DURABILITYSERVICESAMPLES_KEYNAME,
        profile->writerQos.durability_service.max_samples
      ));
    }
  }

  // DurabilityServiceInstances          = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITYSERVICEINSTANCES_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.durability_service.max_instances = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetDurabilityServiceInstancesQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        DURABILITYSERVICEINSTANCES_KEYNAME,
        profile->writerQos.durability_service.max_instances
      ));
    }
  }

  // DurabilityServiceSamplesPerInstance = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITYSERVICESAMPLESPERINSTANCE_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.durability_service.max_samples_per_instance
      = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetDurabilityServiceSamplesPerInstanceQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %s.\n"),
        sectionName.c_str(),
        DURABILITYSERVICESAMPLESPERINSTANCE_KEYNAME,
        profile->writerQos.durability_service.max_samples_per_instance
      ));
    }
  }

  // Deadline                            = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, DEADLINE_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.deadline.period.nanosec = 0;
    profile->writerQos.deadline.period.sec = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetDeadlineQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        DEADLINE_KEYNAME,
        profile->writerQos.deadline.period.sec
      ));
    }
  }

  // LatencyBudget                       = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, LATENCYBUDGET_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.latency_budget.duration.nanosec = 0;
    profile->writerQos.latency_budget.duration.sec = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetLatencyBudgetQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        LATENCYBUDGET_KEYNAME,
        profile->writerQos.latency_budget.duration.sec
      ));
    }
  }

  // LivelinessKind                      = <string> # One of AUTOMATIC, PARTICIPANT, TOPIC
  valueString.clear();
  heap.get_string_value( sectionKey, LIVELINESSKIND_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "AUTOMATIC") {
      profile->writerQos.liveliness.kind = ::DDS::AUTOMATIC_LIVELINESS_QOS;
      profile->writerQosMask |= SetLivelinessKindQos;

    } else if( valueString == "PARTICIPANT") {
      profile->writerQos.liveliness.kind = ::DDS::MANUAL_BY_PARTICIPANT_LIVELINESS_QOS;
      profile->writerQosMask |= SetLivelinessKindQos;

    } else if( valueString == "TOPIC") {
      profile->writerQos.liveliness.kind = ::DDS::MANUAL_BY_TOPIC_LIVELINESS_QOS;
      profile->writerQosMask |= SetLivelinessKindQos;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadPublication() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        LIVELINESSKIND_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %s.\n"),
        sectionName.c_str(),
        LIVELINESSKIND_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // LivelinessDuration                  = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, LIVELINESSDURATION_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.liveliness.lease_duration.nanosec = 0;
    profile->writerQos.liveliness.lease_duration.sec = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetLivelinessDurationQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        LIVELINESSDURATION_KEYNAME,
        profile->writerQos.liveliness.lease_duration.sec
      ));
    }
  }

  // ReliabilityKind                     = <string> # One of BEST_EFFORT, RELIABLE
  valueString.clear();
  heap.get_string_value( sectionKey, RELIABILITYKIND_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "BEST_EFFORT") {
      profile->writerQos.reliability.kind = ::DDS::BEST_EFFORT_RELIABILITY_QOS;
      profile->writerQosMask |= SetReliabilityKindQos;

    } else if( valueString == "RELIABLE") {
      profile->writerQos.reliability.kind = ::DDS::RELIABLE_RELIABILITY_QOS;
      profile->writerQosMask |= SetReliabilityKindQos;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadPublication() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        RELIABILITYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %s.\n"),
        sectionName.c_str(),
        RELIABILITYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // ReliabilityMaxBlocking              = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, RELIABILITYMAXBLOCKING_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.reliability.max_blocking_time.nanosec = 0;
    profile->writerQos.reliability.max_blocking_time.sec = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetReliabilityMaxBlockingQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        RELIABILITYMAXBLOCKING_KEYNAME,
        profile->writerQos.reliability.max_blocking_time.sec
      ));
    }
  }

  // DestinationOrder                    = <string> # One of SOURCE, RECEPTION
  valueString.clear();
  heap.get_string_value( sectionKey, DESTINATIONORDER_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "SOURCE") {
      profile->writerQos.destination_order.kind = ::DDS::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;
      profile->writerQosMask |= SetDestinationOrderQos;

    } else if( valueString == "RECEPTION") {
      profile->writerQos.destination_order.kind = ::DDS::BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS;
      profile->writerQosMask |= SetDestinationOrderQos;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadPublication() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        DESTINATIONORDER_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %s.\n"),
        sectionName.c_str(),
        DESTINATIONORDER_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // HistoryKind                         = <string> # One of ALL, LAST
  valueString.clear();
  heap.get_string_value( sectionKey, HISTORYKIND_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "ALL") {
      profile->writerQos.history.kind = ::DDS::KEEP_ALL_HISTORY_QOS;
      profile->writerQosMask |= SetHistoryKindQos;

    } else if( valueString == "LAST") {
      profile->writerQos.history.kind = ::DDS::KEEP_LAST_HISTORY_QOS;
      profile->writerQosMask |= SetHistoryKindQos;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadPublication() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        HISTORYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %s.\n"),
        sectionName.c_str(),
        HISTORYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // HistoryDepth                        = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, HISTORYDEPTH_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.history.depth = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetHistoryDepthQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        HISTORYDEPTH_KEYNAME,
        profile->writerQos.history.depth
      ));
    }
  }

  // ResourceMaxSamples                  = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, RESOURCEMAXSAMPLES_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.resource_limits.max_samples = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetResourceMaxSamplesQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        RESOURCEMAXSAMPLES_KEYNAME,
        profile->writerQos.resource_limits.max_samples
      ));
    }
  }

  // ResourceMaxInstances                = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, RESOURCEMAXINSTANCES_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.resource_limits.max_instances = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetResourceMaxInstancesQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        RESOURCEMAXINSTANCES_KEYNAME,
        profile->writerQos.resource_limits.max_instances
      ));
    }
  }

  // ResourceMaxSamplesPerInstance       = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, RESOURCEMAXSAMPLESPERINSTANCE_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.resource_limits.max_samples_per_instance = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetResourceMaxSamplesPerInstanceQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        RESOURCEMAXSAMPLESPERINSTANCE_KEYNAME,
        profile->writerQos.resource_limits.max_samples_per_instance
      ));
    }
  }

  // TransportPriority                   = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, TRANSPORTPRIORITY_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.transport_priority.value = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetTransportPriorityQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        TRANSPORTPRIORITY_KEYNAME,
        profile->writerQos.transport_priority.value
      ));
    }
  }

  // Lifespan                            = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, LIFESPANDURATION_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.lifespan.duration.nanosec = 0;
    profile->writerQos.lifespan.duration.sec = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetLifespanQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        LIFESPANDURATION_KEYNAME,
        profile->writerQos.lifespan.duration.sec
      ));
    }
  }

  // UserData                            = <string>
  valueString.clear();
  heap.get_string_value( sectionKey, USERDATA_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQosMask |= SetUserDataQos;
    profile->writerQos.user_data.value.length( valueString.length());
    profile->writerQos.user_data.value.replace(
      valueString.length(),
      valueString.length(),
      const_cast<CORBA::Octet*>(
        reinterpret_cast<const CORBA::Octet*>( valueString.c_str())
      )
    );
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %s.\n"),
        sectionName.c_str(),
        USERDATA_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // #VERSION1.2# OwnershipKind                       = <string> # One of SHARED, EXCLUSIVE
#ifdef VERSION_1_2
  valueString.clear();
  heap.get_string_value( sectionKey, OWNERSHIPKIND_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "SHARED") {
      profile->writerQos.ownership.kind = ::DDS::SHARED_OWNERSHIP_QOS;
      profile->writerQosMask |= SetOwnershipKindQos;

    } else if( valueString == "EXCLUSIVE") {
      profile->writerQos.ownership.kind = ::DDS::EXCLUSIVE_OWNERSHIP_QOS;
      profile->writerQosMask |= SetOwnershipKindQos;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadPublication() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        OWNERSHIPKIND_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [topic/%s] %s == %s.\n"),
        sectionName.c_str(),
        OWNERSHIPKIND_KEYNAME,
        valueString.c_str()
      ));
    }
  }
#endif /* VERSION_1_2 */

  // OwnershipStrength                   = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, OWNERSHIPSTRENGTH_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->writerQos.ownership_strength.value = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetOwnershipStrengthQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        OWNERSHIPSTRENGTH_KEYNAME,
        profile->writerQos.ownership_strength.value
      ));
    }
  }

  // WriterDataLifecycle                 = <bool> # Boolean: numeric 0 or 1
  valueString.clear();
  if( 0 == heap.get_string_value( sectionKey, WRITERDATALIFECYCLE_KEYNAME, valueString)) {
    profile->writerQos.writer_data_lifecycle.autodispose_unregistered_instances
      = ACE_OS::atoi( valueString.c_str());
    profile->writerQosMask |= SetWriterDataLifecycleQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        WRITERDATALIFECYCLE_KEYNAME,
        profile->writerQos.writer_data_lifecycle.autodispose_unregistered_instances
      ));
    }
  }

  // Topic            = <string> # One of topic <name>
  valueString.clear();
  heap.get_string_value( sectionKey, TOPIC_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->topic = valueString.c_str();
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %s.\n"),
        sectionName.c_str(),
        TOPIC_KEYNAME,
        profile->topic.c_str()
      ));
    }
  }

  // TransportIndex   = <number> # Index into transport configurations
  valueString.clear();
  heap.get_string_value( sectionKey, TRANSPORTINDEX_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->transport = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        TRANSPORTINDEX_KEYNAME,
        profile->transport
      ));
    }
  }

  // MessageSource    = <string> # One of subscription <name>
  valueString.clear();
  heap.get_string_value( sectionKey, MESSAGESOURCE_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->source = valueString.c_str();
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %s.\n"),
        sectionName.c_str(),
        MESSAGESOURCE_KEYNAME,
        profile->source.c_str()
      ));
    }
  }

  // MessageRate      = <number> # Samples per second
  valueString.clear();
  heap.get_string_value( sectionKey, MESSAGERATE_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->rate.rate() = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        MESSAGERATE_KEYNAME,
        profile->rate.rate()
      ));
    }
  }

  // MessageSize      = <number> # bytes per sample
  valueString.clear();
  heap.get_string_value( sectionKey, MESSAGESIZE_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->size.mean() = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        MESSAGESIZE_KEYNAME,
        profile->size.mean()
      ));
    }
  }

  // MessageMax       = <number> # upper bound for size
  valueString.clear();
  heap.get_string_value( sectionKey, MESSAGEMAX_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->size.maximum() = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        MESSAGEMAX_KEYNAME,
        profile->size.maximum()
      ));
    }
  }

  // MessageMin       = <number> # lower bound for size
  valueString.clear();
  heap.get_string_value( sectionKey, MESSAGEMIN_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->size.minimum() = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        MESSAGEMIN_KEYNAME,
        profile->size.minimum()
      ));
    }
  }

  // MessageDeviation = <number> # standard deviation for size
  valueString.clear();
  heap.get_string_value( sectionKey, MESSAGEDEVIATION_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->size.deviation() = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadPublication() - ")
        ACE_TEXT("  [publication/%s] %s == %d.\n"),
        sectionName.c_str(),
        MESSAGEDEVIATION_KEYNAME,
        profile->size.deviation()
      ));
    }
  }

  // Store the profile for the current participant.
  this->publicationProfileMap_[ sectionName] = profile;
}

void
Options::loadSubscription(
  ACE_Configuration_Heap& heap,
  ACE_Configuration_Section_Key& sectionKey,
  std::string sectionName
)
{
  /**
   * [subscription/<name>]
   *   # Subscriber Qos Policy values
   *   Presentation                  = <string> # One of INSTANCE, TOPIC, GROUP
   *   PresentationCoherent          = <bool> # Boolean: numeric 0 or 1
   *   PresentationOrdered           = <bool> # Boolean: numeric 0 or 1
   *   Partition                     = <string> # Only single value supported
   *   GroupData                     = <string>
   *   EntityFactory                 = <bool> # Boolean: numeric 0 or 1
   *   # DataReader Qos Policy values
   *   Durability                    = <string> # One of VOLATILE, LOCAL, TRANSIENT, PERSISTENT
   *   Deadline                      = <number>
   *   LatencyBudget                 = <number>
   *   LivelinessKind                = <string> # One of AUTOMATIC, PARTICIPANT, TOPIC
   *   LivelinessDuration            = <number>
   *   ReliabilityKind               = <string> # One of BEST_EFFORT, RELIABLE
   *   ReliabilityMaxBlocking        = <number>
   *   DestinationOrder              = <string> # One of SOURCE, RECEPTION
   *   HistoryKind                   = <string> # One of ALL, LAST
   *   HistoryDepth                  = <number>
   *   ResourceMaxSamples            = <number>
   *   ResourceMaxInstances          = <number>
   *   ResourceMaxSamplesPerInstance = <number>
   *   UserData                      = <string>
   *   TimeBasedFilter               = <number>
   *   ReaderDataLifecycle           = <number>
   *   # Test execution parameters
   *   Topic                         = <string> # One of topic <name>
   *   TransportIndex                = <number> # Index into transport configurations
   *   DataCollectionFile            = <string> # Filename for collected data
   *   DataCollectionBound           = <number>
   *   DataCollectionRetention       = <string> # One of ALL, OLDEST, NEWEST
   */

  // Note that this requires that the Service Participant already be
  // initialized before we configure from the scenario file.  Also,
  // since we have not created any Entities yet, we go to the initial
  // default values rather than to the containing Entity.
  SubscriptionProfile* profile = new SubscriptionProfile();
  profile->subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  profile->readerQos     = TheServiceParticipant->initial_DataReaderQos();
  profile->readerQosMask = 0;
  ACE_TString valueString;

  // Presentation                        = <string> # One of INSTANCE, TOPIC, GROUP
  valueString.clear();
  heap.get_string_value( sectionKey, PRESENTATION_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "INSTANCE") {
      profile->subscriberQos.presentation.access_scope = ::DDS::INSTANCE_PRESENTATION_QOS;

    } else if( valueString == "TOPIC") {
      profile->subscriberQos.presentation.access_scope = ::DDS::TOPIC_PRESENTATION_QOS;

    } else if( valueString == "GROUP") {
      profile->subscriberQos.presentation.access_scope = ::DDS::GROUP_PRESENTATION_QOS;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadSubscription() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        PRESENTATION_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %s.\n"),
        sectionName.c_str(),
        PRESENTATION_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // PresentationCoherent                = <bool> # Boolean: numeric 0 or 1
  valueString.clear();
  if( 0 == heap.get_string_value( sectionKey, PRESENTATIONCOHERENT_KEYNAME, valueString)) {
    profile->subscriberQos.presentation.coherent_access
      = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        PRESENTATIONCOHERENT_KEYNAME,
        profile->subscriberQos.presentation.coherent_access
      ));
    }
  }

  // PresentationOrdered                 = <bool> # Boolean: numeric 0 or 1
  valueString.clear();
  if( 0 == heap.get_string_value( sectionKey, PRESENTATIONORDERED_KEYNAME, valueString)) {
    profile->subscriberQos.presentation.ordered_access
      = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        PRESENTATIONORDERED_KEYNAME,
        profile->subscriberQos.presentation.ordered_access
      ));
    }
  }

  // Partition                           = <string> # Only single value supported
  valueString.clear();
  heap.get_string_value( sectionKey, PARTITION_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->subscriberQos.partition.name.length( 1);
    profile->subscriberQos.partition.name[0] = ACE_OS::strdup( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %s.\n"),
        sectionName.c_str(),
        PARTITION_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // GroupData                           = <string>
  valueString.clear();
  heap.get_string_value( sectionKey, GROUPDATA_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->subscriberQos.group_data.value.length( valueString.length());
    profile->subscriberQos.group_data.value.replace(
      valueString.length(),
      valueString.length(),
      const_cast<CORBA::Octet*>(
        reinterpret_cast<const CORBA::Octet*>( valueString.c_str())
      )
    );
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %s.\n"),
        sectionName.c_str(),
        GROUPDATA_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // EntityFactory                       = <bool> # Boolean: numeric 0 or 1
  valueString.clear();
  if( 0 == heap.get_string_value( sectionKey, ENTITYFACTORY_KEYNAME, valueString)) {
    profile->subscriberQos.entity_factory.autoenable_created_entities
      = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        ENTITYFACTORY_KEYNAME,
        profile->subscriberQos.entity_factory.autoenable_created_entities
      ));
    }
  }

  // Durability = <string> # One of VOLATILE, LOCAL, TRANSIENT, PERSISTENT
  valueString.clear();
  heap.get_string_value( sectionKey, DURABILITY_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "VOLATILE") {
      profile->readerQos.durability.kind = ::DDS::VOLATILE_DURABILITY_QOS;
      profile->readerQosMask |= SetDurabilityQos;

    } else if( valueString == "LOCAL") {
      profile->readerQos.durability.kind = ::DDS::TRANSIENT_LOCAL_DURABILITY_QOS;
      profile->readerQosMask |= SetDurabilityQos;

    } else if( valueString == "TRANSIENT") {
      profile->readerQos.durability.kind = ::DDS::TRANSIENT_DURABILITY_QOS;
      profile->readerQosMask |= SetDurabilityQos;

    } else if( valueString == "PERSISTENT") {
      profile->readerQos.durability.kind = ::DDS::PERSISTENT_DURABILITY_QOS;
      profile->readerQosMask |= SetDurabilityQos;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadSubscription() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        DURABILITY_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %s.\n"),
        sectionName.c_str(),
        DURABILITY_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // Deadline                            = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, DEADLINE_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->readerQos.deadline.period.nanosec = 0;
    profile->readerQos.deadline.period.sec = ACE_OS::atoi( valueString.c_str());
    profile->readerQosMask |= SetDeadlineQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        DEADLINE_KEYNAME,
        profile->readerQos.deadline.period.sec
      ));
    }
  }

  // LatencyBudget                       = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, LATENCYBUDGET_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->readerQos.latency_budget.duration.nanosec = 0;
    profile->readerQos.latency_budget.duration.sec = ACE_OS::atoi( valueString.c_str());
    profile->readerQosMask |= SetLatencyBudgetQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        LATENCYBUDGET_KEYNAME,
        profile->readerQos.latency_budget.duration.sec
      ));
    }
  }

  // LivelinessKind                      = <string> # One of AUTOMATIC, PARTICIPANT, TOPIC
  valueString.clear();
  heap.get_string_value( sectionKey, LIVELINESSKIND_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "AUTOMATIC") {
      profile->readerQos.liveliness.kind = ::DDS::AUTOMATIC_LIVELINESS_QOS;
      profile->readerQosMask |= SetLivelinessKindQos;

    } else if( valueString == "PARTICIPANT") {
      profile->readerQos.liveliness.kind = ::DDS::MANUAL_BY_PARTICIPANT_LIVELINESS_QOS;
      profile->readerQosMask |= SetLivelinessKindQos;

    } else if( valueString == "TOPIC") {
      profile->readerQos.liveliness.kind = ::DDS::MANUAL_BY_TOPIC_LIVELINESS_QOS;
      profile->readerQosMask |= SetLivelinessKindQos;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadSubscription() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        LIVELINESSKIND_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %s.\n"),
        sectionName.c_str(),
        LIVELINESSKIND_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // LivelinessDuration                  = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, LIVELINESSDURATION_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->readerQos.liveliness.lease_duration.nanosec = 0;
    profile->readerQos.liveliness.lease_duration.sec = ACE_OS::atoi( valueString.c_str());
    profile->readerQosMask |= SetLivelinessDurationQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        LIVELINESSDURATION_KEYNAME,
        profile->readerQos.liveliness.lease_duration.sec
      ));
    }
  }

  // ReliabilityKind                     = <string> # One of BEST_EFFORT, RELIABLE
  valueString.clear();
  heap.get_string_value( sectionKey, RELIABILITYKIND_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "BEST_EFFORT") {
      profile->readerQos.reliability.kind = ::DDS::BEST_EFFORT_RELIABILITY_QOS;
      profile->readerQosMask |= SetReliabilityKindQos;

    } else if( valueString == "RELIABLE") {
      profile->readerQos.reliability.kind = ::DDS::RELIABLE_RELIABILITY_QOS;
      profile->readerQosMask |= SetReliabilityKindQos;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadSubscription() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        RELIABILITYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %s.\n"),
        sectionName.c_str(),
        RELIABILITYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // ReliabilityMaxBlocking              = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, RELIABILITYMAXBLOCKING_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->readerQos.reliability.max_blocking_time.nanosec = 0;
    profile->readerQos.reliability.max_blocking_time.sec = ACE_OS::atoi( valueString.c_str());
    profile->readerQosMask |= SetReliabilityMaxBlockingQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        RELIABILITYMAXBLOCKING_KEYNAME,
        profile->readerQos.reliability.max_blocking_time.sec
      ));
    }
  }

  // DestinationOrder                    = <string> # One of SOURCE, RECEPTION
  valueString.clear();
  heap.get_string_value( sectionKey, DESTINATIONORDER_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "SOURCE") {
      profile->readerQos.destination_order.kind = ::DDS::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;
      profile->readerQosMask |= SetDestinationOrderQos;

    } else if( valueString == "RECEPTION") {
      profile->readerQos.destination_order.kind = ::DDS::BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS;
      profile->readerQosMask |= SetDestinationOrderQos;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadSubscription() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        DESTINATIONORDER_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %s.\n"),
        sectionName.c_str(),
        DESTINATIONORDER_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // HistoryKind                         = <string> # One of ALL, LAST
  valueString.clear();
  heap.get_string_value( sectionKey, HISTORYKIND_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "ALL") {
      profile->readerQos.history.kind = ::DDS::KEEP_ALL_HISTORY_QOS;
      profile->readerQosMask |= SetHistoryKindQos;

    } else if( valueString == "LAST") {
      profile->readerQos.history.kind = ::DDS::KEEP_LAST_HISTORY_QOS;
      profile->readerQosMask |= SetHistoryKindQos;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadSubscription() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        HISTORYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %s.\n"),
        sectionName.c_str(),
        HISTORYKIND_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // HistoryDepth                        = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, HISTORYDEPTH_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->readerQos.history.depth = ACE_OS::atoi( valueString.c_str());
    profile->readerQosMask |= SetHistoryDepthQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        HISTORYDEPTH_KEYNAME,
        profile->readerQos.history.depth
      ));
    }
  }

  // ResourceMaxSamples                  = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, RESOURCEMAXSAMPLES_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->readerQos.resource_limits.max_samples = ACE_OS::atoi( valueString.c_str());
    profile->readerQosMask |= SetResourceMaxSamplesQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        RESOURCEMAXSAMPLES_KEYNAME,
        profile->readerQos.resource_limits.max_samples
      ));
    }
  }

  // ResourceMaxInstances                = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, RESOURCEMAXINSTANCES_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->readerQos.resource_limits.max_instances = ACE_OS::atoi( valueString.c_str());
    profile->readerQosMask |= SetResourceMaxInstancesQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        RESOURCEMAXINSTANCES_KEYNAME,
        profile->readerQos.resource_limits.max_instances
      ));
    }
  }

  // ResourceMaxSamplesPerInstance       = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, RESOURCEMAXSAMPLESPERINSTANCE_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->readerQos.resource_limits.max_samples_per_instance = ACE_OS::atoi( valueString.c_str());
    profile->readerQosMask |= SetResourceMaxSamplesPerInstanceQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        RESOURCEMAXSAMPLESPERINSTANCE_KEYNAME,
        profile->readerQos.resource_limits.max_samples_per_instance
      ));
    }
  }

  // UserData                            = <string>
  valueString.clear();
  heap.get_string_value( sectionKey, USERDATA_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->readerQosMask |= SetUserDataQos;
    profile->readerQos.user_data.value.length( valueString.length());
    profile->readerQos.user_data.value.replace(
      valueString.length(),
      valueString.length(),
      const_cast<CORBA::Octet*>(
        reinterpret_cast<const CORBA::Octet*>( valueString.c_str())
      )
    );
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %s.\n"),
        sectionName.c_str(),
        USERDATA_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // TimeBasedFilter               = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, TIMEBASEDFILTER_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->readerQos.time_based_filter.minimum_separation.nanosec = 0;
    profile->readerQos.time_based_filter.minimum_separation.sec
      = ACE_OS::atoi( valueString.c_str());
    profile->readerQosMask |= SetTimeBasedFilterQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        TIMEBASEDFILTER_KEYNAME,
        profile->readerQos.time_based_filter.minimum_separation.sec
      ));
    }
  }

  // ReaderDataLifecycle           = <number>
  valueString.clear();
  if( 0 == heap.get_string_value( sectionKey, READERDATALIFECYCLE_KEYNAME, valueString)) {
    profile->readerQos.reader_data_lifecycle.autopurge_nowriter_samples_delay.nanosec = 0;
    profile->readerQos.reader_data_lifecycle.autopurge_nowriter_samples_delay.sec
      = ACE_OS::atoi( valueString.c_str());
    profile->readerQosMask |= SetReaderDataLifecycleQos;
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        READERDATALIFECYCLE_KEYNAME,
        profile->readerQos.reader_data_lifecycle.autopurge_nowriter_samples_delay.sec
      ));
    }
  }

  // Topic            = <string> # One of topic <name>
  valueString.clear();
  heap.get_string_value( sectionKey, TOPIC_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->topic = valueString.c_str();
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %s.\n"),
        sectionName.c_str(),
        TOPIC_KEYNAME,
        profile->topic.c_str()
      ));
    }
  }

  // TransportIndex   = <number> # Index into transport configurations
  valueString.clear();
  heap.get_string_value( sectionKey, TRANSPORTINDEX_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->transport = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        TRANSPORTINDEX_KEYNAME,
        profile->transport
      ));
    }
  }

  // DataCollectionFile            = <string> # Filename for collected data
  valueString.clear();
  heap.get_string_value( sectionKey, DATACOLLECTIONFILE_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->datafile = valueString.c_str();
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %s.\n"),
        sectionName.c_str(),
        DATACOLLECTIONFILE_KEYNAME,
        profile->datafile.c_str()
      ));
    }
  }

  // DataCollectionBound           = <number>
  valueString.clear();
  heap.get_string_value( sectionKey, DATACOLLECTIONBOUND_KEYNAME, valueString);
  if (valueString.length() > 0) {
    profile->bound = ACE_OS::atoi( valueString.c_str());
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %d.\n"),
        sectionName.c_str(),
        DATACOLLECTIONBOUND_KEYNAME,
        profile->bound
      ));
    }
  }

  // DataCollectionRetention       = <string> # One of ALL, OLDEST, NEWEST
  valueString.clear();
  heap.get_string_value( sectionKey, DATACOLLECTIONRETENTION_KEYNAME, valueString);
  if (valueString.length() > 0) {
    if( valueString == "ALL") {
      profile->retention = ::OpenDDS::DCPS::DataCollector<double>::Unbounded;

    } else if( valueString == "OLDEST") {
      profile->retention = ::OpenDDS::DCPS::DataCollector<double>::KeepOldest;

    } else if( valueString == "NEWEST") {
      profile->retention = ::OpenDDS::DCPS::DataCollector<double>::KeepNewest;

    } else {
      ACE_DEBUG((LM_WARNING,
        ACE_TEXT("(%P|%t) loadSubscription() - ")
        ACE_TEXT("unrecognized value for %s: %s - ")
        ACE_TEXT("not assigning a value.\n"),
        DATACOLLECTIONRETENTION_KEYNAME,
        valueString.c_str()
      ));
    }
    if( this->verbose()) {
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) Options::loadSubscription() - ")
        ACE_TEXT("  [subscription/%s] %s == %s.\n"),
        sectionName.c_str(),
        DATACOLLECTIONRETENTION_KEYNAME,
        valueString.c_str()
      ));
    }
  }

  // Store the profile for the current participant.
  this->subscriptionProfileMap_[ sectionName] = profile;
}

} // End of namespace Test


