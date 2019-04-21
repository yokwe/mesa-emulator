#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("stub/BulkData1");

#include "../stub/BulkData1.h"

#include "../courier/Last.h"


Courier::BulkData1::Descriptor::Descriptor_CHOICE_01& Courier::BulkData1::Descriptor::null() const {
    if (choiceTag == CHOICE_TAG::null) {
        return choice_01;
    } else {
        logger.error("choiceTag  expect null  actual %s", Courier::toString(choiceTag));
        COURIER_FATAL_ERROR();
    }
}

Courier::BulkData1::Descriptor::Descriptor_CHOICE_01& Courier::BulkData1::Descriptor::immediate() const {
    if (choiceTag == CHOICE_TAG::immediate) {
        return choice_01;
    } else {
        logger.error("choiceTag  expect immediate  actual %s", Courier::toString(choiceTag));
        COURIER_FATAL_ERROR();
    }
}

Courier::BulkData1::Descriptor::Descriptor_CHOICE_02& Courier::BulkData1::Descriptor::passive() const {
    if (choiceTag == CHOICE_TAG::passive) {
        return choice_02;
    } else {
        logger.error("choiceTag  expect passive  actual %s", Courier::toString(choiceTag));
        COURIER_FATAL_ERROR();
    }
}

Courier::BulkData1::Descriptor::Descriptor_CHOICE_02& Courier::BulkData1::Descriptor::active() const {
    if (choiceTag == CHOICE_TAG::active) {
        return choice_02;
    } else {
        logger.error("choiceTag  expect active  actual %s", Courier::toString(choiceTag));
        COURIER_FATAL_ERROR();
    }
}

//
// Enum Function Definition
//

QString Courier::toString(const Courier::BulkData1::Descriptor::CHOICE_TAG value) {
    static QMap<Courier::BulkData1::Descriptor::CHOICE_TAG, QString> map = {
        {Courier::BulkData1::Descriptor::CHOICE_TAG::null,      "null"},
        {Courier::BulkData1::Descriptor::CHOICE_TAG::immediate, "immediate"},
        {Courier::BulkData1::Descriptor::CHOICE_TAG::passive,   "passive"},
        {Courier::BulkData1::Descriptor::CHOICE_TAG::active,    "active"},
    };

    if (map.contains(value)) {
        return map[value];
    } else {
        return QString("%1").arg((quint16)value);
    }
}


void Courier::serialize(BLOCK& block, const Courier::BulkData1::Descriptor::CHOICE_TAG value) {
    Courier::serialize(block, (quint16)value);
}


void Courier::deserialize(BLOCK& block, Courier::BulkData1::Descriptor::CHOICE_TAG& value) {
    quint16 t;
    Courier::deserialize(block, t);
    value = (Courier::BulkData1::Descriptor::CHOICE_TAG)t;
}

//
// Record Function Definition
//
QString Courier::toString(const Courier::BulkData1::Identifier& value) {
    QStringList list;
    // host  [ARRAY (3)UNSPECIFIED]]
    list << QString("[%1 %2]").arg("host").arg(Courier::toString(value.host));
    // hostRelativeIdentifier  [ARRAY (2)UNSPECIFIED]]
    list << QString("[%1 %2]").arg("hostRelativeIdentifier").arg(Courier::toString(value.hostRelativeIdentifier));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::BulkData1::Descriptor::Descriptor_CHOICE_01&) {
    return "[]";
}
QString Courier::toString(const Courier::BulkData1::Descriptor::Descriptor_CHOICE_02& value) {
    QStringList list;
    // network  [ARRAY (2)UNSPECIFIED]]
    list << QString("[%1 %2]").arg("network").arg(Courier::toString(value.network));
    // host  [ARRAY (3)UNSPECIFIED]]
    list << QString("[%1 %2]").arg("host").arg(Courier::toString(value.host));
    // identifier  [REF BulkData1::Identifier]
    //   RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("identifier").arg(Courier::toString(value.identifier));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::BulkData1::InvalidDescriptor&) {
    return "[]";
}
QString Courier::toString(const Courier::BulkData1::NoSuchIdentifier&) {
    return "[]";
}
QString Courier::toString(const Courier::BulkData1::IdentifierBusy&) {
    return "[]";
}
QString Courier::toString(const Courier::BulkData1::WrongHost&) {
    return "[]";
}
QString Courier::toString(const Courier::BulkData1::WrongDirection&) {
    return "[]";
}
QString Courier::toString(const Courier::BulkData1::TransferAborted&) {
    return "[]";
}
QString Courier::toString(const Courier::BulkData1::Send::Param& value) {
    QStringList list;
    // identifier  [REF BulkData1::Identifier]
    //   RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("identifier").arg(Courier::toString(value.identifier));
    // sink  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    list << QString("[%1 %2]").arg("sink").arg(Courier::toString(value.sink));
    // timeout  CARDINAL
    list << QString("[%1 %2]").arg("timeout").arg(Courier::toString(value.timeout));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::BulkData1::Send::Result&) {
    return "[]";
}
QString Courier::toString(const Courier::BulkData1::Receive::Param& value) {
    QStringList list;
    // identifier  [REF BulkData1::Identifier]
    //   RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("identifier").arg(Courier::toString(value.identifier));
    // source  [REF BulkData1::Source]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    list << QString("[%1 %2]").arg("source").arg(Courier::toString(value.source));
    // timeout  CARDINAL
    list << QString("[%1 %2]").arg("timeout").arg(Courier::toString(value.timeout));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::BulkData1::Receive::Result&) {
    return "[]";
}
QString Courier::toString(const Courier::BulkData1::Cancel::Param& value) {
    QStringList list;
    // identifier  [REF BulkData1::Identifier]
    //   RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("identifier").arg(Courier::toString(value.identifier));
    // timeout  CARDINAL
    list << QString("[%1 %2]").arg("timeout").arg(Courier::toString(value.timeout));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::BulkData1::Cancel::Result&) {
    return "[]";
}

void Courier::serialize(BLOCK& block, const Courier::BulkData1::Identifier& value) {
    // host  [ARRAY (3)UNSPECIFIED]]
    Courier::serialize(block, value.host);
    // hostRelativeIdentifier  [ARRAY (2)UNSPECIFIED]]
    Courier::serialize(block, value.hostRelativeIdentifier);
}
void Courier::serialize(BLOCK&, const Courier::BulkData1::Descriptor::Descriptor_CHOICE_01&) {
}
void Courier::serialize(BLOCK& block, const Courier::BulkData1::Descriptor::Descriptor_CHOICE_02& value) {
    // network  [ARRAY (2)UNSPECIFIED]]
    Courier::serialize(block, value.network);
    // host  [ARRAY (3)UNSPECIFIED]]
    Courier::serialize(block, value.host);
    // identifier  [REF BulkData1::Identifier]
    //   RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]]
    Courier::serialize(block, value.identifier);
}
void Courier::serialize(BLOCK&, const Courier::BulkData1::InvalidDescriptor&) {
}
void Courier::serialize(BLOCK&, const Courier::BulkData1::NoSuchIdentifier&) {
}
void Courier::serialize(BLOCK&, const Courier::BulkData1::IdentifierBusy&) {
}
void Courier::serialize(BLOCK&, const Courier::BulkData1::WrongHost&) {
}
void Courier::serialize(BLOCK&, const Courier::BulkData1::WrongDirection&) {
}
void Courier::serialize(BLOCK&, const Courier::BulkData1::TransferAborted&) {
}
void Courier::serialize(BLOCK& block, const Courier::BulkData1::Send::Param& value) {
    // identifier  [REF BulkData1::Identifier]
    //   RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]]
    Courier::serialize(block, value.identifier);
    // sink  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::serialize(block, value.sink);
    // timeout  CARDINAL
    Courier::serialize(block, value.timeout);
}
void Courier::serialize(BLOCK&, const Courier::BulkData1::Send::Result&) {
}
void Courier::serialize(BLOCK& block, const Courier::BulkData1::Receive::Param& value) {
    // identifier  [REF BulkData1::Identifier]
    //   RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]]
    Courier::serialize(block, value.identifier);
    // source  [REF BulkData1::Source]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::serialize(block, value.source);
    // timeout  CARDINAL
    Courier::serialize(block, value.timeout);
}
void Courier::serialize(BLOCK&, const Courier::BulkData1::Receive::Result&) {
}
void Courier::serialize(BLOCK& block, const Courier::BulkData1::Cancel::Param& value) {
    // identifier  [REF BulkData1::Identifier]
    //   RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]]
    Courier::serialize(block, value.identifier);
    // timeout  CARDINAL
    Courier::serialize(block, value.timeout);
}
void Courier::serialize(BLOCK&, const Courier::BulkData1::Cancel::Result&) {
}

void Courier::deserialize(BLOCK& block, Courier::BulkData1::Identifier& value) {
    // host  [ARRAY (3)UNSPECIFIED]]
    Courier::deserialize(block, value.host);
    // hostRelativeIdentifier  [ARRAY (2)UNSPECIFIED]]
    Courier::deserialize(block, value.hostRelativeIdentifier);
}
void Courier::deserialize(BLOCK&, Courier::BulkData1::Descriptor::Descriptor_CHOICE_01&) {
}
void Courier::deserialize(BLOCK& block, Courier::BulkData1::Descriptor::Descriptor_CHOICE_02& value) {
    // network  [ARRAY (2)UNSPECIFIED]]
    Courier::deserialize(block, value.network);
    // host  [ARRAY (3)UNSPECIFIED]]
    Courier::deserialize(block, value.host);
    // identifier  [REF BulkData1::Identifier]
    //   RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]]
    Courier::deserialize(block, value.identifier);
}
void Courier::deserialize(BLOCK&, Courier::BulkData1::InvalidDescriptor&) {
}
void Courier::deserialize(BLOCK&, Courier::BulkData1::NoSuchIdentifier&) {
}
void Courier::deserialize(BLOCK&, Courier::BulkData1::IdentifierBusy&) {
}
void Courier::deserialize(BLOCK&, Courier::BulkData1::WrongHost&) {
}
void Courier::deserialize(BLOCK&, Courier::BulkData1::WrongDirection&) {
}
void Courier::deserialize(BLOCK&, Courier::BulkData1::TransferAborted&) {
}
void Courier::deserialize(BLOCK& block, Courier::BulkData1::Send::Param& value) {
    // identifier  [REF BulkData1::Identifier]
    //   RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]]
    Courier::deserialize(block, value.identifier);
    // sink  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::deserialize(block, value.sink);
    // timeout  CARDINAL
    Courier::deserialize(block, value.timeout);
}
void Courier::deserialize(BLOCK&, Courier::BulkData1::Send::Result&) {
}
void Courier::deserialize(BLOCK& block, Courier::BulkData1::Receive::Param& value) {
    // identifier  [REF BulkData1::Identifier]
    //   RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]]
    Courier::deserialize(block, value.identifier);
    // source  [REF BulkData1::Source]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::deserialize(block, value.source);
    // timeout  CARDINAL
    Courier::deserialize(block, value.timeout);
}
void Courier::deserialize(BLOCK&, Courier::BulkData1::Receive::Result&) {
}
void Courier::deserialize(BLOCK& block, Courier::BulkData1::Cancel::Param& value) {
    // identifier  [REF BulkData1::Identifier]
    //   RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]]
    Courier::deserialize(block, value.identifier);
    // timeout  CARDINAL
    Courier::deserialize(block, value.timeout);
}
void Courier::deserialize(BLOCK&, Courier::BulkData1::Cancel::Result&) {
}

//
// Choice Function Definition
//
QString Courier::toString(const Courier::BulkData1::Descriptor& value) {
    switch(value.choiceTag) {
    case Courier::BulkData1::Descriptor::CHOICE_TAG::null:
        return QString("[%1 %2]").arg("null").arg(Courier::toString(value.null()));
    case Courier::BulkData1::Descriptor::CHOICE_TAG::immediate:
        return QString("[%1 %2]").arg("immediate").arg(Courier::toString(value.immediate()));
    case Courier::BulkData1::Descriptor::CHOICE_TAG::passive:
        return QString("[%1 %2]").arg("passive").arg(Courier::toString(value.passive()));
    case Courier::BulkData1::Descriptor::CHOICE_TAG::active:
        return QString("[%1 %2]").arg("active").arg(Courier::toString(value.active()));
    default:
        logger.error("Unexpected choiceTag = %d", (quint16)value.choiceTag);
        COURIER_FATAL_ERROR();
        break;
    }
}

void Courier::serialize(BLOCK& block, const Courier::BulkData1::Descriptor& value) {
    block.serialize16((quint16)value.choiceTag);
    switch(value.choiceTag) {
    case Courier::BulkData1::Descriptor::CHOICE_TAG::null:
        Courier::serialize(block, value.null());
        break;
    case Courier::BulkData1::Descriptor::CHOICE_TAG::immediate:
        Courier::serialize(block, value.immediate());
        break;
    case Courier::BulkData1::Descriptor::CHOICE_TAG::passive:
        Courier::serialize(block, value.passive());
        break;
    case Courier::BulkData1::Descriptor::CHOICE_TAG::active:
        Courier::serialize(block, value.active());
        break;
    default:
        logger.error("Unexpected choiceTag = %d", (quint16)value.choiceTag);
        COURIER_FATAL_ERROR();
        break;
    }
}

void Courier::deserialize(BLOCK& block, Courier::BulkData1::Descriptor& value) {
    quint16 choiceTag_;
    block.deserialize16(choiceTag_);
    value.choiceTag = (Courier::BulkData1::Descriptor::CHOICE_TAG)choiceTag_;
    switch(value.choiceTag) {
    case Courier::BulkData1::Descriptor::CHOICE_TAG::null:
        Courier::deserialize(block, value.null());
        break;
    case Courier::BulkData1::Descriptor::CHOICE_TAG::immediate:
        Courier::deserialize(block, value.immediate());
        break;
    case Courier::BulkData1::Descriptor::CHOICE_TAG::passive:
        Courier::deserialize(block, value.passive());
        break;
    case Courier::BulkData1::Descriptor::CHOICE_TAG::active:
        Courier::deserialize(block, value.active());
        break;
    default:
        logger.error("Unexpected choiceTag = %d", (quint16)value.choiceTag);
        COURIER_FATAL_ERROR();
        break;
    }
}
