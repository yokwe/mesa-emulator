#include "../util/Util.h"
static log4cpp::Category& logger = Logger::getLogger("stub/Clearinghouse2");

#include "../stub/Clearinghouse2.h"

#include "../courier/Last.h"


//
// Enum Function Definition
//

QString Courier::toString(const Courier::Clearinghouse2::WhichArgument value) {
    static QMap<Courier::Clearinghouse2::WhichArgument, QString> map = {
        {Courier::Clearinghouse2::WhichArgument::first,  "first"},
        {Courier::Clearinghouse2::WhichArgument::second, "second"},
    };

    if (map.contains(value)) {
        return map[value];
    } else {
        return QString("%1").arg((quint16)value);
    }
}

QString Courier::toString(const Courier::Clearinghouse2::ArgumentProblem value) {
    static QMap<Courier::Clearinghouse2::ArgumentProblem, QString> map = {
        {Courier::Clearinghouse2::ArgumentProblem::illegalProperty,         "illegalProperty"},
        {Courier::Clearinghouse2::ArgumentProblem::illegalOrganizationName, "illegalOrganizationName"},
        {Courier::Clearinghouse2::ArgumentProblem::illegalDomainName,       "illegalDomainName"},
        {Courier::Clearinghouse2::ArgumentProblem::illegalObjectName,       "illegalObjectName"},
        {Courier::Clearinghouse2::ArgumentProblem::noSuchOrganization,      "noSuchOrganization"},
        {Courier::Clearinghouse2::ArgumentProblem::noSuchDomain,            "noSuchDomain"},
        {Courier::Clearinghouse2::ArgumentProblem::noSuchObject,            "noSuchObject"},
    };

    if (map.contains(value)) {
        return map[value];
    } else {
        return QString("%1").arg((quint16)value);
    }
}

QString Courier::toString(const Courier::Clearinghouse2::CallProblem value) {
    static QMap<Courier::Clearinghouse2::CallProblem, QString> map = {
        {Courier::Clearinghouse2::CallProblem::accessRightsInsufficient, "accessRightsInsufficient"},
        {Courier::Clearinghouse2::CallProblem::tooBusy,                  "tooBusy"},
        {Courier::Clearinghouse2::CallProblem::serverDown,               "serverDown"},
        {Courier::Clearinghouse2::CallProblem::useCourier,               "useCourier"},
        {Courier::Clearinghouse2::CallProblem::other,                    "other"},
    };

    if (map.contains(value)) {
        return map[value];
    } else {
        return QString("%1").arg((quint16)value);
    }
}

QString Courier::toString(const Courier::Clearinghouse2::PropertyProblem value) {
    static QMap<Courier::Clearinghouse2::PropertyProblem, QString> map = {
        {Courier::Clearinghouse2::PropertyProblem::missing,   "missing"},
        {Courier::Clearinghouse2::PropertyProblem::wrongType, "wrongType"},
    };

    if (map.contains(value)) {
        return map[value];
    } else {
        return QString("%1").arg((quint16)value);
    }
}

QString Courier::toString(const Courier::Clearinghouse2::UpdateProblem value) {
    static QMap<Courier::Clearinghouse2::UpdateProblem, QString> map = {
        {Courier::Clearinghouse2::UpdateProblem::noChange,         "noChange"},
        {Courier::Clearinghouse2::UpdateProblem::outOfDate,        "outOfDate"},
        {Courier::Clearinghouse2::UpdateProblem::objectOverflow,   "objectOverflow"},
        {Courier::Clearinghouse2::UpdateProblem::databaseOverflow, "databaseOverflow"},
    };

    if (map.contains(value)) {
        return map[value];
    } else {
        return QString("%1").arg((quint16)value);
    }
}


void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::WhichArgument value) {
    Courier::serialize(block, (quint16)value);
}

void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::ArgumentProblem value) {
    Courier::serialize(block, (quint16)value);
}

void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::CallProblem value) {
    Courier::serialize(block, (quint16)value);
}

void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::PropertyProblem value) {
    Courier::serialize(block, (quint16)value);
}

void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::UpdateProblem value) {
    Courier::serialize(block, (quint16)value);
}


void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::WhichArgument& value) {
    quint16 t;
    Courier::deserialize(block, t);
    value = (Courier::Clearinghouse2::WhichArgument)t;
}

void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::ArgumentProblem& value) {
    quint16 t;
    Courier::deserialize(block, t);
    value = (Courier::Clearinghouse2::ArgumentProblem)t;
}

void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::CallProblem& value) {
    quint16 t;
    Courier::deserialize(block, t);
    value = (Courier::Clearinghouse2::CallProblem)t;
}

void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::PropertyProblem& value) {
    quint16 t;
    Courier::deserialize(block, t);
    value = (Courier::Clearinghouse2::PropertyProblem)t;
}

void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::UpdateProblem& value) {
    quint16 t;
    Courier::deserialize(block, t);
    value = (Courier::Clearinghouse2::UpdateProblem)t;
}

//
// Record Function Definition
//
QString Courier::toString(const Courier::Clearinghouse2::TwoPartName& value) {
    QStringList list;
    // organization  [REF Clearinghouse2::Organization]
    //   STRING
    list << QString("[%1 %2]").arg("organization").arg(Courier::toString(value.organization));
    // domain  [REF Clearinghouse2::Domain]
    //   STRING
    list << QString("[%1 %2]").arg("domain").arg(Courier::toString(value.domain));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ThreePartName& value) {
    QStringList list;
    // organization  [REF Clearinghouse2::Organization]
    //   STRING
    list << QString("[%1 %2]").arg("organization").arg(Courier::toString(value.organization));
    // domain  [REF Clearinghouse2::Domain]
    //   STRING
    list << QString("[%1 %2]").arg("domain").arg(Courier::toString(value.domain));
    // object  [REF Clearinghouse2::Object]
    //   STRING
    list << QString("[%1 %2]").arg("object").arg(Courier::toString(value.object));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::NetworkAddress& value) {
    QStringList list;
    // network  [ARRAY (2)UNSPECIFIED]]
    list << QString("[%1 %2]").arg("network").arg(Courier::toString(value.network));
    // host  [ARRAY (3)UNSPECIFIED]]
    list << QString("[%1 %2]").arg("host").arg(Courier::toString(value.host));
    // socket  UNSPECIFIED
    list << QString("[%1 %2]").arg("socket").arg(Courier::toString(value.socket));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::Authenticator& value) {
    QStringList list;
    // credentials  [REF Authentication1::Credentials]
    //   RECORD [type: CARDINAL, value: [SEQUENCE (65535)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("credentials").arg(Courier::toString(value.credentials));
    // verifier  [REF Authentication1::Verifier]
    //   [SEQUENCE (12)UNSPECIFIED]]
    list << QString("[%1 %2]").arg("verifier").arg(Courier::toString(value.verifier));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ArgumentError& value) {
    QStringList list;
    // problem  [REF Clearinghouse2::ArgumentProblem]
    //   ENUM [[illegalProperty  10], [illegalOrganizationName  11], [illegalDomainName  12], [illegalObjectName  13], [noSuchOrganization  14], [noSuchDomain  15], [noSuchObject  16]]
    list << QString("[%1 %2]").arg("problem").arg(Courier::toString(value.problem));
    // which  [REF Clearinghouse2::WhichArgument]
    //   ENUM [[first  1], [second  2]]
    list << QString("[%1 %2]").arg("which").arg(Courier::toString(value.which));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::AuthenticationError& value) {
    QStringList list;
    // problem  [REF Authentication1::Problem]
    //   ENUM [[credentialsInvalid  0], [verifierInvalid  1], [verifierExpired  2], [verifierReused  3], [credentialsExpired  4], [inappropriateCredentials  5]]
    list << QString("[%1 %2]").arg("problem").arg(Courier::toString(value.problem));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::CallError& value) {
    QStringList list;
    // problem  [REF Clearinghouse2::CallProblem]
    //   ENUM [[accessRightsInsufficient  1], [tooBusy  2], [serverDown  3], [useCourier  4], [other  5]]
    list << QString("[%1 %2]").arg("problem").arg(Courier::toString(value.problem));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::PropertyError& value) {
    QStringList list;
    // problem  [REF Clearinghouse2::PropertyProblem]
    //   ENUM [[missing  20], [wrongType  21]]
    list << QString("[%1 %2]").arg("problem").arg(Courier::toString(value.problem));
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::UpdateError& value) {
    QStringList list;
    // problem  [REF Clearinghouse2::UpdateProblem]
    //   ENUM [[noChange  30], [outOfDate  31], [objectOverflow  32], [databaseOverflow  33]]
    list << QString("[%1 %2]").arg("problem").arg(Courier::toString(value.problem));
    // found  BOOLEAN
    list << QString("[%1 %2]").arg("found").arg(Courier::toString(value.found));
    // which  [REF Clearinghouse2::WhichArgument]
    //   ENUM [[first  1], [second  2]]
    list << QString("[%1 %2]").arg("which").arg(Courier::toString(value.which));
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::WrongServer& value) {
    QStringList list;
    // hint  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("hint").arg(Courier::toString(value.hint));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::CreateObject::Param& value) {
    QStringList list;
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("name").arg(Courier::toString(value.name));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::CreateObject::Result&) {
    return "[]";
}
QString Courier::toString(const Courier::Clearinghouse2::DeleteObject::Param& value) {
    QStringList list;
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("name").arg(Courier::toString(value.name));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::DeleteObject::Result&) {
    return "[]";
}
QString Courier::toString(const Courier::Clearinghouse2::LookupObject::Param& value) {
    QStringList list;
    // name  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("name").arg(Courier::toString(value.name));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::LookupObject::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ListOrganizations::Param& value) {
    QStringList list;
    // pattern  [REF Clearinghouse2::OrganizationNamePattern]
    //   STRING
    list << QString("[%1 %2]").arg("pattern").arg(Courier::toString(value.pattern));
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    list << QString("[%1 %2]").arg("list").arg(Courier::toString(value.list));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ListOrganizations::Result&) {
    return "[]";
}
QString Courier::toString(const Courier::Clearinghouse2::ListDomain::Param& value) {
    QStringList list;
    // pattern  [REF Clearinghouse2::DomainNamePattern]
    //   RECORD [organization: STRING, domain: STRING]
    list << QString("[%1 %2]").arg("pattern").arg(Courier::toString(value.pattern));
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    list << QString("[%1 %2]").arg("list").arg(Courier::toString(value.list));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ListDomain::Result&) {
    return "[]";
}
QString Courier::toString(const Courier::Clearinghouse2::ListObjects::Param& value) {
    QStringList list;
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("pattern").arg(Courier::toString(value.pattern));
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    list << QString("[%1 %2]").arg("property").arg(Courier::toString(value.property));
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    list << QString("[%1 %2]").arg("list").arg(Courier::toString(value.list));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ListObjects::Result&) {
    return "[]";
}
QString Courier::toString(const Courier::Clearinghouse2::ListAliasesOf::Param& value) {
    QStringList list;
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("pattern").arg(Courier::toString(value.pattern));
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    list << QString("[%1 %2]").arg("list").arg(Courier::toString(value.list));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ListAliasesOf::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::CreateAlias::Param& value) {
    QStringList list;
    // alias  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("alias").arg(Courier::toString(value.alias));
    // sameAs  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("sameAs").arg(Courier::toString(value.sameAs));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::CreateAlias::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::DeleteAlias::Param& value) {
    QStringList list;
    // alias  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("alias").arg(Courier::toString(value.alias));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::DeleteAlias::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ListAliases::Param& value) {
    QStringList list;
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("pattern").arg(Courier::toString(value.pattern));
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    list << QString("[%1 %2]").arg("list").arg(Courier::toString(value.list));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ListAliases::Result&) {
    return "[]";
}
QString Courier::toString(const Courier::Clearinghouse2::DeleteProperty::Param& value) {
    QStringList list;
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("name").arg(Courier::toString(value.name));
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    list << QString("[%1 %2]").arg("property").arg(Courier::toString(value.property));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::DeleteProperty::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ListProperties::Param& value) {
    QStringList list;
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("pattern").arg(Courier::toString(value.pattern));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ListProperties::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    // properties  [REF Clearinghouse2::Properties]
    //   [SEQUENCE (250)LONG_CARDINAL]]
    list << QString("[%1 %2]").arg("properties").arg(Courier::toString(value.properties));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::AddItemProperty::Param& value) {
    QStringList list;
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("name").arg(Courier::toString(value.name));
    // newProperty  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    list << QString("[%1 %2]").arg("newProperty").arg(Courier::toString(value.newProperty));
    // value  [REF Clearinghouse2::Item]
    //   [SEQUENCE (500)UNSPECIFIED]]
    list << QString("[%1 %2]").arg("value").arg(Courier::toString(value.value));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::AddItemProperty::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::RetrieveItem::Param& value) {
    QStringList list;
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("pattern").arg(Courier::toString(value.pattern));
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    list << QString("[%1 %2]").arg("property").arg(Courier::toString(value.property));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::RetrieveItem::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    // value  [REF Clearinghouse2::Item]
    //   [SEQUENCE (500)UNSPECIFIED]]
    list << QString("[%1 %2]").arg("value").arg(Courier::toString(value.value));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ChangeItem::Param& value) {
    QStringList list;
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("name").arg(Courier::toString(value.name));
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    list << QString("[%1 %2]").arg("property").arg(Courier::toString(value.property));
    // newValue  [REF Clearinghouse2::Item]
    //   [SEQUENCE (500)UNSPECIFIED]]
    list << QString("[%1 %2]").arg("newValue").arg(Courier::toString(value.newValue));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ChangeItem::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::AddGroupProperty::Param& value) {
    QStringList list;
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("name").arg(Courier::toString(value.name));
    // newProperty  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    list << QString("[%1 %2]").arg("newProperty").arg(Courier::toString(value.newProperty));
    // membership  [REF BulkData1::Source]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    list << QString("[%1 %2]").arg("membership").arg(Courier::toString(value.membership));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::AddGroupProperty::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::RetrieveMembers::Param& value) {
    QStringList list;
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("pattern").arg(Courier::toString(value.pattern));
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    list << QString("[%1 %2]").arg("property").arg(Courier::toString(value.property));
    // membership  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    list << QString("[%1 %2]").arg("membership").arg(Courier::toString(value.membership));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::RetrieveMembers::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::AddMember::Param& value) {
    QStringList list;
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("name").arg(Courier::toString(value.name));
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    list << QString("[%1 %2]").arg("property").arg(Courier::toString(value.property));
    // newMember  [REF Clearinghouse2::ThreePartName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("newMember").arg(Courier::toString(value.newMember));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::AddMember::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::AddSelf::Param& value) {
    QStringList list;
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("name").arg(Courier::toString(value.name));
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    list << QString("[%1 %2]").arg("property").arg(Courier::toString(value.property));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::AddSelf::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::DeleteMember::Param& value) {
    QStringList list;
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("name").arg(Courier::toString(value.name));
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    list << QString("[%1 %2]").arg("property").arg(Courier::toString(value.property));
    // member  [REF Clearinghouse2::ThreePartName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("member").arg(Courier::toString(value.member));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::DeleteMember::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::DeleteSelf::Param& value) {
    QStringList list;
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("name").arg(Courier::toString(value.name));
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    list << QString("[%1 %2]").arg("property").arg(Courier::toString(value.property));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::DeleteSelf::Result& value) {
    QStringList list;
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::IsMember::Param& value) {
    QStringList list;
    // memberOf  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("memberOf").arg(Courier::toString(value.memberOf));
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    list << QString("[%1 %2]").arg("property").arg(Courier::toString(value.property));
    // secondaryProperty  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    list << QString("[%1 %2]").arg("secondaryProperty").arg(Courier::toString(value.secondaryProperty));
    // name  [REF Clearinghouse2::ThreePartName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("name").arg(Courier::toString(value.name));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::IsMember::Result& value) {
    QStringList list;
    // isMember  BOOLEAN
    list << QString("[%1 %2]").arg("isMember").arg(Courier::toString(value.isMember));
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    list << QString("[%1 %2]").arg("distinguishedObject").arg(Courier::toString(value.distinguishedObject));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::RetrieveAddresses::Param&) {
    return "[]";
}
QString Courier::toString(const Courier::Clearinghouse2::RetrieveAddresses::Result& value) {
    QStringList list;
    // address  [REF Clearinghouse2::NetworkAddressList]
    //   [SEQUENCE (40)RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], socket: UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("address").arg(Courier::toString(value.address));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ListDomainServed::Param& value) {
    QStringList list;
    // domains  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    list << QString("[%1 %2]").arg("domains").arg(Courier::toString(value.domains));
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    list << QString("[%1 %2]").arg("agent").arg(Courier::toString(value.agent));
    return QString("[%1]").arg(list.join(" "));
}
QString Courier::toString(const Courier::Clearinghouse2::ListDomainServed::Result&) {
    return "[]";
}

void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::TwoPartName& value) {
    // organization  [REF Clearinghouse2::Organization]
    //   STRING
    Courier::serialize(block, value.organization);
    // domain  [REF Clearinghouse2::Domain]
    //   STRING
    Courier::serialize(block, value.domain);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::ThreePartName& value) {
    // organization  [REF Clearinghouse2::Organization]
    //   STRING
    Courier::serialize(block, value.organization);
    // domain  [REF Clearinghouse2::Domain]
    //   STRING
    Courier::serialize(block, value.domain);
    // object  [REF Clearinghouse2::Object]
    //   STRING
    Courier::serialize(block, value.object);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::NetworkAddress& value) {
    // network  [ARRAY (2)UNSPECIFIED]]
    Courier::serialize(block, value.network);
    // host  [ARRAY (3)UNSPECIFIED]]
    Courier::serialize(block, value.host);
    // socket  UNSPECIFIED
    Courier::serialize(block, value.socket);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::Authenticator& value) {
    // credentials  [REF Authentication1::Credentials]
    //   RECORD [type: CARDINAL, value: [SEQUENCE (65535)UNSPECIFIED]]]
    Courier::serialize(block, value.credentials);
    // verifier  [REF Authentication1::Verifier]
    //   [SEQUENCE (12)UNSPECIFIED]]
    Courier::serialize(block, value.verifier);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::ArgumentError& value) {
    // problem  [REF Clearinghouse2::ArgumentProblem]
    //   ENUM [[illegalProperty  10], [illegalOrganizationName  11], [illegalDomainName  12], [illegalObjectName  13], [noSuchOrganization  14], [noSuchDomain  15], [noSuchObject  16]]
    Courier::serialize(block, value.problem);
    // which  [REF Clearinghouse2::WhichArgument]
    //   ENUM [[first  1], [second  2]]
    Courier::serialize(block, value.which);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::AuthenticationError& value) {
    // problem  [REF Authentication1::Problem]
    //   ENUM [[credentialsInvalid  0], [verifierInvalid  1], [verifierExpired  2], [verifierReused  3], [credentialsExpired  4], [inappropriateCredentials  5]]
    Courier::serialize(block, value.problem);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::CallError& value) {
    // problem  [REF Clearinghouse2::CallProblem]
    //   ENUM [[accessRightsInsufficient  1], [tooBusy  2], [serverDown  3], [useCourier  4], [other  5]]
    Courier::serialize(block, value.problem);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::PropertyError& value) {
    // problem  [REF Clearinghouse2::PropertyProblem]
    //   ENUM [[missing  20], [wrongType  21]]
    Courier::serialize(block, value.problem);
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::UpdateError& value) {
    // problem  [REF Clearinghouse2::UpdateProblem]
    //   ENUM [[noChange  30], [outOfDate  31], [objectOverflow  32], [databaseOverflow  33]]
    Courier::serialize(block, value.problem);
    // found  BOOLEAN
    Courier::serialize(block, value.found);
    // which  [REF Clearinghouse2::WhichArgument]
    //   ENUM [[first  1], [second  2]]
    Courier::serialize(block, value.which);
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::WrongServer& value) {
    // hint  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.hint);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::CreateObject::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.name);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK&, const Courier::Clearinghouse2::CreateObject::Result&) {
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::DeleteObject::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.name);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK&, const Courier::Clearinghouse2::DeleteObject::Result&) {
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::LookupObject::Param& value) {
    // name  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.name);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::LookupObject::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::ListOrganizations::Param& value) {
    // pattern  [REF Clearinghouse2::OrganizationNamePattern]
    //   STRING
    Courier::serialize(block, value.pattern);
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::serialize(block, value.list);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK&, const Courier::Clearinghouse2::ListOrganizations::Result&) {
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::ListDomain::Param& value) {
    // pattern  [REF Clearinghouse2::DomainNamePattern]
    //   RECORD [organization: STRING, domain: STRING]
    Courier::serialize(block, value.pattern);
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::serialize(block, value.list);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK&, const Courier::Clearinghouse2::ListDomain::Result&) {
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::ListObjects::Param& value) {
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.pattern);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::serialize(block, value.property);
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::serialize(block, value.list);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK&, const Courier::Clearinghouse2::ListObjects::Result&) {
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::ListAliasesOf::Param& value) {
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.pattern);
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::serialize(block, value.list);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::ListAliasesOf::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::CreateAlias::Param& value) {
    // alias  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.alias);
    // sameAs  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.sameAs);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::CreateAlias::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::DeleteAlias::Param& value) {
    // alias  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.alias);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::DeleteAlias::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::ListAliases::Param& value) {
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.pattern);
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::serialize(block, value.list);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK&, const Courier::Clearinghouse2::ListAliases::Result&) {
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::DeleteProperty::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.name);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::serialize(block, value.property);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::DeleteProperty::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::ListProperties::Param& value) {
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.pattern);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::ListProperties::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
    // properties  [REF Clearinghouse2::Properties]
    //   [SEQUENCE (250)LONG_CARDINAL]]
    Courier::serialize(block, value.properties);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::AddItemProperty::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.name);
    // newProperty  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::serialize(block, value.newProperty);
    // value  [REF Clearinghouse2::Item]
    //   [SEQUENCE (500)UNSPECIFIED]]
    Courier::serialize(block, value.value);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::AddItemProperty::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::RetrieveItem::Param& value) {
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.pattern);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::serialize(block, value.property);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::RetrieveItem::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
    // value  [REF Clearinghouse2::Item]
    //   [SEQUENCE (500)UNSPECIFIED]]
    Courier::serialize(block, value.value);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::ChangeItem::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.name);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::serialize(block, value.property);
    // newValue  [REF Clearinghouse2::Item]
    //   [SEQUENCE (500)UNSPECIFIED]]
    Courier::serialize(block, value.newValue);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::ChangeItem::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::AddGroupProperty::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.name);
    // newProperty  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::serialize(block, value.newProperty);
    // membership  [REF BulkData1::Source]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::serialize(block, value.membership);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::AddGroupProperty::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::RetrieveMembers::Param& value) {
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.pattern);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::serialize(block, value.property);
    // membership  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::serialize(block, value.membership);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::RetrieveMembers::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::AddMember::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.name);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::serialize(block, value.property);
    // newMember  [REF Clearinghouse2::ThreePartName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.newMember);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::AddMember::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::AddSelf::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.name);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::serialize(block, value.property);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::AddSelf::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::DeleteMember::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.name);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::serialize(block, value.property);
    // member  [REF Clearinghouse2::ThreePartName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.member);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::DeleteMember::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::DeleteSelf::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.name);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::serialize(block, value.property);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::DeleteSelf::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::IsMember::Param& value) {
    // memberOf  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.memberOf);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::serialize(block, value.property);
    // secondaryProperty  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::serialize(block, value.secondaryProperty);
    // name  [REF Clearinghouse2::ThreePartName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.name);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::IsMember::Result& value) {
    // isMember  BOOLEAN
    Courier::serialize(block, value.isMember);
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::serialize(block, value.distinguishedObject);
}
void Courier::serialize(BLOCK&, const Courier::Clearinghouse2::RetrieveAddresses::Param&) {
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::RetrieveAddresses::Result& value) {
    // address  [REF Clearinghouse2::NetworkAddressList]
    //   [SEQUENCE (40)RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], socket: UNSPECIFIED]]]
    Courier::serialize(block, value.address);
}
void Courier::serialize(BLOCK& block, const Courier::Clearinghouse2::ListDomainServed::Param& value) {
    // domains  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::serialize(block, value.domains);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::serialize(block, value.agent);
}
void Courier::serialize(BLOCK&, const Courier::Clearinghouse2::ListDomainServed::Result&) {
}

void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::TwoPartName& value) {
    // organization  [REF Clearinghouse2::Organization]
    //   STRING
    Courier::deserialize(block, value.organization);
    // domain  [REF Clearinghouse2::Domain]
    //   STRING
    Courier::deserialize(block, value.domain);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::ThreePartName& value) {
    // organization  [REF Clearinghouse2::Organization]
    //   STRING
    Courier::deserialize(block, value.organization);
    // domain  [REF Clearinghouse2::Domain]
    //   STRING
    Courier::deserialize(block, value.domain);
    // object  [REF Clearinghouse2::Object]
    //   STRING
    Courier::deserialize(block, value.object);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::NetworkAddress& value) {
    // network  [ARRAY (2)UNSPECIFIED]]
    Courier::deserialize(block, value.network);
    // host  [ARRAY (3)UNSPECIFIED]]
    Courier::deserialize(block, value.host);
    // socket  UNSPECIFIED
    Courier::deserialize(block, value.socket);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::Authenticator& value) {
    // credentials  [REF Authentication1::Credentials]
    //   RECORD [type: CARDINAL, value: [SEQUENCE (65535)UNSPECIFIED]]]
    Courier::deserialize(block, value.credentials);
    // verifier  [REF Authentication1::Verifier]
    //   [SEQUENCE (12)UNSPECIFIED]]
    Courier::deserialize(block, value.verifier);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::ArgumentError& value) {
    // problem  [REF Clearinghouse2::ArgumentProblem]
    //   ENUM [[illegalProperty  10], [illegalOrganizationName  11], [illegalDomainName  12], [illegalObjectName  13], [noSuchOrganization  14], [noSuchDomain  15], [noSuchObject  16]]
    Courier::deserialize(block, value.problem);
    // which  [REF Clearinghouse2::WhichArgument]
    //   ENUM [[first  1], [second  2]]
    Courier::deserialize(block, value.which);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::AuthenticationError& value) {
    // problem  [REF Authentication1::Problem]
    //   ENUM [[credentialsInvalid  0], [verifierInvalid  1], [verifierExpired  2], [verifierReused  3], [credentialsExpired  4], [inappropriateCredentials  5]]
    Courier::deserialize(block, value.problem);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::CallError& value) {
    // problem  [REF Clearinghouse2::CallProblem]
    //   ENUM [[accessRightsInsufficient  1], [tooBusy  2], [serverDown  3], [useCourier  4], [other  5]]
    Courier::deserialize(block, value.problem);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::PropertyError& value) {
    // problem  [REF Clearinghouse2::PropertyProblem]
    //   ENUM [[missing  20], [wrongType  21]]
    Courier::deserialize(block, value.problem);
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::UpdateError& value) {
    // problem  [REF Clearinghouse2::UpdateProblem]
    //   ENUM [[noChange  30], [outOfDate  31], [objectOverflow  32], [databaseOverflow  33]]
    Courier::deserialize(block, value.problem);
    // found  BOOLEAN
    Courier::deserialize(block, value.found);
    // which  [REF Clearinghouse2::WhichArgument]
    //   ENUM [[first  1], [second  2]]
    Courier::deserialize(block, value.which);
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::WrongServer& value) {
    // hint  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.hint);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::CreateObject::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.name);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK&, Courier::Clearinghouse2::CreateObject::Result&) {
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::DeleteObject::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.name);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK&, Courier::Clearinghouse2::DeleteObject::Result&) {
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::LookupObject::Param& value) {
    // name  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.name);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::LookupObject::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::ListOrganizations::Param& value) {
    // pattern  [REF Clearinghouse2::OrganizationNamePattern]
    //   STRING
    Courier::deserialize(block, value.pattern);
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::deserialize(block, value.list);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK&, Courier::Clearinghouse2::ListOrganizations::Result&) {
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::ListDomain::Param& value) {
    // pattern  [REF Clearinghouse2::DomainNamePattern]
    //   RECORD [organization: STRING, domain: STRING]
    Courier::deserialize(block, value.pattern);
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::deserialize(block, value.list);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK&, Courier::Clearinghouse2::ListDomain::Result&) {
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::ListObjects::Param& value) {
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.pattern);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::deserialize(block, value.property);
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::deserialize(block, value.list);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK&, Courier::Clearinghouse2::ListObjects::Result&) {
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::ListAliasesOf::Param& value) {
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.pattern);
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::deserialize(block, value.list);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::ListAliasesOf::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::CreateAlias::Param& value) {
    // alias  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.alias);
    // sameAs  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.sameAs);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::CreateAlias::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::DeleteAlias::Param& value) {
    // alias  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.alias);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::DeleteAlias::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::ListAliases::Param& value) {
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.pattern);
    // list  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::deserialize(block, value.list);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK&, Courier::Clearinghouse2::ListAliases::Result&) {
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::DeleteProperty::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.name);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::deserialize(block, value.property);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::DeleteProperty::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::ListProperties::Param& value) {
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.pattern);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::ListProperties::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
    // properties  [REF Clearinghouse2::Properties]
    //   [SEQUENCE (250)LONG_CARDINAL]]
    Courier::deserialize(block, value.properties);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::AddItemProperty::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.name);
    // newProperty  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::deserialize(block, value.newProperty);
    // value  [REF Clearinghouse2::Item]
    //   [SEQUENCE (500)UNSPECIFIED]]
    Courier::deserialize(block, value.value);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::AddItemProperty::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::RetrieveItem::Param& value) {
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.pattern);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::deserialize(block, value.property);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::RetrieveItem::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
    // value  [REF Clearinghouse2::Item]
    //   [SEQUENCE (500)UNSPECIFIED]]
    Courier::deserialize(block, value.value);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::ChangeItem::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.name);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::deserialize(block, value.property);
    // newValue  [REF Clearinghouse2::Item]
    //   [SEQUENCE (500)UNSPECIFIED]]
    Courier::deserialize(block, value.newValue);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::ChangeItem::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::AddGroupProperty::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.name);
    // newProperty  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::deserialize(block, value.newProperty);
    // membership  [REF BulkData1::Source]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::deserialize(block, value.membership);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::AddGroupProperty::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::RetrieveMembers::Param& value) {
    // pattern  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.pattern);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::deserialize(block, value.property);
    // membership  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::deserialize(block, value.membership);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::RetrieveMembers::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::AddMember::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.name);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::deserialize(block, value.property);
    // newMember  [REF Clearinghouse2::ThreePartName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.newMember);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::AddMember::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::AddSelf::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.name);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::deserialize(block, value.property);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::AddSelf::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::DeleteMember::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.name);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::deserialize(block, value.property);
    // member  [REF Clearinghouse2::ThreePartName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.member);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::DeleteMember::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::DeleteSelf::Param& value) {
    // name  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.name);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::deserialize(block, value.property);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::DeleteSelf::Result& value) {
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::IsMember::Param& value) {
    // memberOf  [REF Clearinghouse2::ObjectNamePattern]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.memberOf);
    // property  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::deserialize(block, value.property);
    // secondaryProperty  [REF Clearinghouse2::Property]
    //   LONG_CARDINAL
    Courier::deserialize(block, value.secondaryProperty);
    // name  [REF Clearinghouse2::ThreePartName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.name);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::IsMember::Result& value) {
    // isMember  BOOLEAN
    Courier::deserialize(block, value.isMember);
    // distinguishedObject  [REF Clearinghouse2::ObjectName]
    //   RECORD [organization: STRING, domain: STRING, object: STRING]
    Courier::deserialize(block, value.distinguishedObject);
}
void Courier::deserialize(BLOCK&, Courier::Clearinghouse2::RetrieveAddresses::Param&) {
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::RetrieveAddresses::Result& value) {
    // address  [REF Clearinghouse2::NetworkAddressList]
    //   [SEQUENCE (40)RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], socket: UNSPECIFIED]]]
    Courier::deserialize(block, value.address);
}
void Courier::deserialize(BLOCK& block, Courier::Clearinghouse2::ListDomainServed::Param& value) {
    // domains  [REF BulkData1::Sink]
    //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
    Courier::deserialize(block, value.domains);
    // agent  [REF Clearinghouse2::Authenticator]
    //   RECORD [credentials: RECORD [type: [REF Authentication1::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
    Courier::deserialize(block, value.agent);
}
void Courier::deserialize(BLOCK&, Courier::Clearinghouse2::ListDomainServed::Result&) {
}
