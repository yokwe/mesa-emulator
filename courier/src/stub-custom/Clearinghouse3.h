#ifndef STUB_Clearinghouse3_H__
#define STUB_Clearinghouse3_H__

#include "../courier/Courier.h"
#include "../stub/BulkData1.h"
#include "../stub/Authentication3.h"

namespace Courier {
    namespace Clearinghouse3 {

        const quint32 PROGRAM_NUMBER = 2;
        const quint32 VERSION_NUMBER = 3;

        // TYPE  Organization  STRING
        using Organization = STRING;

        // TYPE  Domain  STRING
        using Domain = STRING;

        // TYPE  Object  STRING
        using Object = STRING;

        // CONST  maxOrganizationsLength  20  CARDINAL
        const CARDINAL maxOrganizationsLength = 20;

        // CONST  maxDomainLength  20  CARDINAL
        const CARDINAL maxDomainLength = 20;

        // CONST  maxObjectLength  40  CARDINAL
        const CARDINAL maxObjectLength = 40;

        // TYPE  OrganizationName  [REF Clearinghouse3::Organization]
        //   STRING
        using OrganizationName = Organization;

        // TYPE  TwoPartName  RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain]]
        //   RECORD [organization: STRING, domain: STRING]
        struct TwoPartName {
            Organization organization;
            Domain       domain;
        };

        // TYPE  DomainName  [REF Clearinghouse3::TwoPartName]
        //   RECORD [organization: STRING, domain: STRING]
        using DomainName = TwoPartName;

        // TYPE  ThreePartName  RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]
        //   RECORD [organization: STRING, domain: STRING, object: STRING]
        struct ThreePartName {
            Organization organization;
            Domain       domain;
            Object       object;
        };

        // TYPE  ObjectName  [REF Clearinghouse3::ThreePartName]
        //   RECORD [organization: STRING, domain: STRING, object: STRING]
        using ObjectName = ThreePartName;

        // TYPE  Name  [REF Clearinghouse3::ThreePartName]
        //   RECORD [organization: STRING, domain: STRING, object: STRING]
        using Name = ThreePartName;

        // TYPE  OrganizationNamePattern  [REF Clearinghouse3::Organization]
        //   STRING
        using OrganizationNamePattern = Organization;

        // TYPE  DomainNamePattern  [REF Clearinghouse3::TwoPartName]
        //   RECORD [organization: STRING, domain: STRING]
        using DomainNamePattern = TwoPartName;

        // TYPE  ObjectNamePattern  [REF Clearinghouse3::ThreePartName]
        //   RECORD [organization: STRING, domain: STRING, object: STRING]
        using ObjectNamePattern = ThreePartName;

        // TYPE  StreamOfDomain  [CHOICE [[[[nextSegment  0]] => RECORD [segment: [SEQUENCE (65535)[REF Clearinghouse3::Domain]]], restOfStream: [REF Clearinghouse3::StreamOfDomain]]], [[[lastSegment  1]] => [SEQUENCE (65535)[REF Clearinghouse3::Domain]]]]]]
        using StreamOfDomain = StreamOf<Domain>;

        // TYPE  StreamOfDomainName  [CHOICE [[[[nextSegment  0]] => RECORD [segment: [SEQUENCE (65535)[REF Clearinghouse3::DomainName]]], restOfStream: [REF Clearinghouse3::StreamOfDomainName]]], [[[lastSegment  1]] => [SEQUENCE (65535)[REF Clearinghouse3::DomainName]]]]]]
        using StreamOfDomainName = StreamOf<DomainName>;

        // TYPE  StreamOfObject  [CHOICE [[[[nextSegment  0]] => RECORD [segment: [SEQUENCE (65535)[REF Clearinghouse3::Object]]], restOfStream: [REF Clearinghouse3::StreamOfObject]]], [[[lastSegment  1]] => [SEQUENCE (65535)[REF Clearinghouse3::Object]]]]]]
        using StreamOfObject= StreamOf<Object>;

        // TYPE  StreamOfObjectName  [CHOICE [[[[nextSegment  0]] => RECORD [segment: [SEQUENCE (65535)[REF Clearinghouse3::ObjectName]]], restOfStream: [REF Clearinghouse3::StreamOfObjectName]]], [[[lastSegment  1]] => [SEQUENCE (65535)[REF Clearinghouse3::ObjectName]]]]]]
        using StreamOfObjectName= StreamOf<ObjectName>;

        // TYPE  StreamOfOrganization  [CHOICE [[[[nextSegment  0]] => RECORD [segment: [SEQUENCE (65535)[REF Clearinghouse3::Organization]]], restOfStream: [REF Clearinghouse3::StreamOfOrganization]]], [[[lastSegment  1]] => [SEQUENCE (65535)[REF Clearinghouse3::Organization]]]]]]
        using StreamOfOrganization= StreamOf<Organization>;

        // TYPE  StreamOfThreePartName  [CHOICE [[[[nextSegment  0]] => RECORD [segment: [SEQUENCE (65535)[REF Clearinghouse3::ThreePartName]]], restOfStream: [REF Clearinghouse3::StreamOfThreePartName]]], [[[lastSegment  1]] => [SEQUENCE (65535)[REF Clearinghouse3::ThreePartName]]]]]]
        using StreamOfThreePartName= StreamOf<ThreePartName>;

        // TYPE  Property  LONG_CARDINAL
        using Property = LONG_CARDINAL;

        // TYPE  Properties  [SEQUENCE (250)[REF Clearinghouse3::Property]]]
        //   [SEQUENCE (250)LONG_CARDINAL]]
        using Properties = SEQUENCE_N<Property,250>;

        // CONST  all  0  [REF Clearinghouse3::Property]
        //   LONG_CARDINAL
        const Property all = 0;

        // CONST  nullProperty  4294967295  [REF Clearinghouse3::Property]
        //   LONG_CARDINAL
        const Property nullProperty = 0xFFFFFFFFU;

        // TYPE  Item  [SEQUENCE (500)UNSPECIFIED]]
        using Item = SEQUENCE_N<UNSPECIFIED,500>;

        // TYPE  NetworkAddress  RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], socket: UNSPECIFIED]
        struct NetworkAddress {
            ARRAY_N<UNSPECIFIED,2> network;
            ARRAY_N<UNSPECIFIED,3> host;
            UNSPECIFIED            socket;
        };

        // TYPE  NetworkAddressList  [SEQUENCE (40)[REF Clearinghouse3::NetworkAddress]]]
        //   [SEQUENCE (40)RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], socket: UNSPECIFIED]]]
        using NetworkAddressList = SEQUENCE_N<NetworkAddress,40>;

        // TYPE  Authenticator  RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]
        //   RECORD [credentials: RECORD [type: [REF Authentication3::CredentialsType], value: [SEQUENCE (65535)UNSPECIFIED]]], verifier: [SEQUENCE (12)UNSPECIFIED]]]
        struct Authenticator {
            Authentication3::Credentials credentials;
            Authentication3::Verifier    verifier;
        };

        // CONST  wildcard  ""*""  STRING
        const STRING wildcard = "*";

        // TYPE  WhichArgument  ENUM [[first  1], [second  2]]
        enum class WhichArgument : quint16 {
            first  = 1,
            second = 2,
        };

        // TYPE  ArgumentProblem  ENUM [[illegalProperty  10], [illegalOrganizationName  11], [illegalDomainName  12], [illegalObjectName  13], [noSuchOrganization  14], [noSuchDomain  15], [noSuchObject  16]]
        enum class ArgumentProblem : quint16 {
            illegalProperty         = 10,
            illegalOrganizationName = 11,
            illegalDomainName       = 12,
            illegalObjectName       = 13,
            noSuchOrganization      = 14,
            noSuchDomain            = 15,
            noSuchObject            = 16,
        };

        // CONST  ArgumentError  2  [ERROR problem: [REF Clearinghouse3::ArgumentProblem], which: [REF Clearinghouse3::WhichArgument]]
        //   [ERROR problem: ENUM [[illegalProperty  10], [illegalOrganizationName  11], [illegalDomainName  12], [illegalObjectName  13], [noSuchOrganization  14], [noSuchDomain  15], [noSuchObject  16]], which: ENUM [[first  1], [second  2]]]
        struct ArgumentError {
            static const quint16 CODE = 2;

            ArgumentProblem problem;
            WhichArgument   which;
        };

        // CONST  AuthenticationError  6  [ERROR problem: [REF Authentication3::Problem]]
        //   [ERROR problem: ENUM [[credentialsInvalid  0], [verifierInvalid  1], [verifierExpired  2], [verifierReused  3], [credentialsExpired  4], [inappropriateCredentials  5], [proxyInvalid  6], [proxyExpired  7], [otherProblem  8]]]
        struct AuthenticationError {
            static const quint16 CODE = 6;

            Authentication3::Problem problem;
        };

        // TYPE  CallProblem  ENUM [[accessRightsInsufficient  1], [tooBusy  2], [serverDown  3], [useCourier  4], [other  5]]
        enum class CallProblem : quint16 {
            accessRightsInsufficient = 1,
            tooBusy                  = 2,
            serverDown               = 3,
            useCourier               = 4,
            other                    = 5,
        };

        // CONST  CallError  1  [ERROR problem: [REF Clearinghouse3::CallProblem]]
        //   [ERROR problem: ENUM [[accessRightsInsufficient  1], [tooBusy  2], [serverDown  3], [useCourier  4], [other  5]]]
        struct CallError {
            static const quint16 CODE = 1;

            CallProblem problem;
        };

        // TYPE  PropertyProblem  ENUM [[missing  20], [wrongType  21]]
        enum class PropertyProblem : quint16 {
            missing   = 20,
            wrongType = 21,
        };

        // CONST  PropertyError  3  [ERROR problem: [REF Clearinghouse3::PropertyProblem], distinguishedObject: [REF Clearinghouse3::ObjectName]]
        //   [ERROR problem: ENUM [[missing  20], [wrongType  21]], distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]]
        struct PropertyError {
            static const quint16 CODE = 3;

            PropertyProblem problem;
            ObjectName      distinguishedObject;
        };

        // TYPE  UpdateProblem  ENUM [[noChange  30], [outOfDate  31], [objectOverflow  32], [databaseOverflow  33]]
        enum class UpdateProblem : quint16 {
            noChange         = 30,
            outOfDate        = 31,
            objectOverflow   = 32,
            databaseOverflow = 33,
        };

        // CONST  UpdateError  4  [ERROR problem: [REF Clearinghouse3::UpdateProblem], found: BOOLEAN, which: [REF Clearinghouse3::WhichArgument], distinguishedObject: [REF Clearinghouse3::ObjectName]]
        //   [ERROR problem: ENUM [[noChange  30], [outOfDate  31], [objectOverflow  32], [databaseOverflow  33]], found: BOOLEAN, which: ENUM [[first  1], [second  2]], distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]]
        struct UpdateError {
            static const quint16 CODE = 4;

            UpdateProblem problem;
            BOOLEAN       found;
            WhichArgument which;
            ObjectName    distinguishedObject;
        };

        // CONST  WrongServer  5  [ERROR hint: [REF Clearinghouse3::ObjectName]]
        //   [ERROR hint: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]]
        struct WrongServer {
            static const quint16 CODE = 5;

            ObjectName hint;
        };

        // CONST  CreateObject  2  [PROCEDURE arg:[name: [REF Clearinghouse3::ObjectName], agent: [REF Clearinghouse3::Authenticator]] result:[] error:[ArgumentError, AuthenticationError, CallError, UpdateError, WrongServer]]
        //   [PROCEDURE arg:[name: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[] error:[]]
        struct CreateObject {
            struct Param {
                ObjectName    name;
                Authenticator agent;
            };
            struct Result {
            };

            static const quint16 CODE = 2;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, UpdateError, WrongServer
        };

        // CONST  DeleteObject  3  [PROCEDURE arg:[name: [REF Clearinghouse3::ObjectName], agent: [REF Clearinghouse3::Authenticator]] result:[] error:[ArgumentError, AuthenticationError, CallError, UpdateError, WrongServer]]
        //   [PROCEDURE arg:[name: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[] error:[]]
        struct DeleteObject {
            struct Param {
                ObjectName    name;
                Authenticator agent;
            };
            struct Result {
            };

            static const quint16 CODE = 3;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, UpdateError, WrongServer
        };

        // CONST  LookupObject  4  [PROCEDURE arg:[name: [REF Clearinghouse3::ObjectNamePattern], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName]] error:[ArgumentError, AuthenticationError, CallError, WrongServer]]
        //   [PROCEDURE arg:[name: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]] error:[]]
        struct LookupObject {
            struct Param {
                ObjectNamePattern name;
                Authenticator     agent;
            };
            struct Result {
                ObjectName distinguishedObject;
            };

            static const quint16 CODE = 4;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, WrongServer
        };

        // CONST  ListOrganizations  5  [PROCEDURE arg:[pattern: [REF Clearinghouse3::OrganizationNamePattern], list: [REF BulkData1::Sink], agent: [REF Clearinghouse3::Authenticator]] result:[] error:[ArgumentError, AuthenticationError, CallError, WrongServer]]
        //   [PROCEDURE arg:[pattern: STRING, list: [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[] error:[]]
        struct ListOrganizations {
            struct Param {
                OrganizationNamePattern pattern;
                BulkData1::Sink         list;
                Authenticator           agent;
            };
            struct Result {
            };

            static const quint16 CODE = 5;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, WrongServer
        };

        // CONST  ListDomain  6  [PROCEDURE arg:[pattern: [REF Clearinghouse3::DomainNamePattern], list: [REF BulkData1::Sink], agent: [REF Clearinghouse3::Authenticator]] result:[] error:[ArgumentError, AuthenticationError, CallError, WrongServer]]
        //   [PROCEDURE arg:[pattern: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain]], list: [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[] error:[]]
        struct ListDomain {
            struct Param {
                DomainNamePattern pattern;
                BulkData1::Sink   list;
                Authenticator     agent;
            };
            struct Result {
            };

            static const quint16 CODE = 6;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, WrongServer
        };

        // CONST  ListObjects  7  [PROCEDURE arg:[pattern: [REF Clearinghouse3::ObjectNamePattern], property: [REF Clearinghouse3::Property], list: [REF BulkData1::Sink], agent: [REF Clearinghouse3::Authenticator]] result:[] error:[ArgumentError, AuthenticationError, CallError, WrongServer]]
        //   [PROCEDURE arg:[pattern: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], property: LONG_CARDINAL, list: [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[] error:[]]
        struct ListObjects {
            struct Param {
                ObjectNamePattern pattern;
                Property          property;
                BulkData1::Sink   list;
                Authenticator     agent;
            };
            struct Result {
            };

            static const quint16 CODE = 7;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, WrongServer
        };

        // CONST  ListAliasesOf  9  [PROCEDURE arg:[pattern: [REF Clearinghouse3::ObjectNamePattern], list: [REF BulkData1::Sink], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName]] error:[ArgumentError, AuthenticationError, CallError, WrongServer]]
        //   [PROCEDURE arg:[pattern: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], list: [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]] error:[]]
        struct ListAliasesOf {
            struct Param {
                ObjectNamePattern pattern;
                BulkData1::Sink   list;
                Authenticator     agent;
            };
            struct Result {
                ObjectName distinguishedObject;
            };

            static const quint16 CODE = 9;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, WrongServer
        };

        // CONST  CreateAlias  10  [PROCEDURE arg:[alias: [REF Clearinghouse3::ObjectName], sameAs: [REF Clearinghouse3::ObjectName], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName]] error:[ArgumentError, AuthenticationError, CallError, UpdateError, WrongServer]]
        //   [PROCEDURE arg:[alias: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], sameAs: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]] error:[]]
        struct CreateAlias {
            struct Param {
                ObjectName    alias;
                ObjectName    sameAs;
                Authenticator agent;
            };
            struct Result {
                ObjectName distinguishedObject;
            };

            static const quint16 CODE = 10;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, UpdateError, WrongServer
        };

        // CONST  DeleteAlias  11  [PROCEDURE arg:[alias: [REF Clearinghouse3::ObjectName], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName]] error:[ArgumentError, AuthenticationError, CallError, UpdateError, WrongServer]]
        //   [PROCEDURE arg:[alias: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]] error:[]]
        struct DeleteAlias {
            struct Param {
                ObjectName    alias;
                Authenticator agent;
            };
            struct Result {
                ObjectName distinguishedObject;
            };

            static const quint16 CODE = 11;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, UpdateError, WrongServer
        };

        // CONST  ListAliases  8  [PROCEDURE arg:[pattern: [REF Clearinghouse3::ObjectNamePattern], list: [REF BulkData1::Sink], agent: [REF Clearinghouse3::Authenticator]] result:[] error:[ArgumentError, AuthenticationError, CallError, WrongServer]]
        //   [PROCEDURE arg:[pattern: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], list: [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[] error:[]]
        struct ListAliases {
            struct Param {
                ObjectNamePattern pattern;
                BulkData1::Sink   list;
                Authenticator     agent;
            };
            struct Result {
            };

            static const quint16 CODE = 8;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, WrongServer
        };

        // CONST  DeleteProperty  14  [PROCEDURE arg:[name: [REF Clearinghouse3::ObjectName], property: [REF Clearinghouse3::Property], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName]] error:[ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer]]
        //   [PROCEDURE arg:[name: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], property: LONG_CARDINAL, agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]] error:[]]
        struct DeleteProperty {
            struct Param {
                ObjectName    name;
                Property      property;
                Authenticator agent;
            };
            struct Result {
                ObjectName distinguishedObject;
            };

            static const quint16 CODE = 14;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer
        };

        // CONST  ListProperties  15  [PROCEDURE arg:[pattern: [REF Clearinghouse3::ObjectNamePattern], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName], properties: [REF Clearinghouse3::Properties]] error:[ArgumentError, AuthenticationError, CallError, WrongServer]]
        //   [PROCEDURE arg:[pattern: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], properties: [SEQUENCE (250)[REF Clearinghouse3::Property]]]] error:[]]
        struct ListProperties {
            struct Param {
                ObjectNamePattern pattern;
                Authenticator     agent;
            };
            struct Result {
                ObjectName distinguishedObject;
                Properties properties;
            };

            static const quint16 CODE = 15;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, WrongServer
        };

        // CONST  AddItemProperty  13  [PROCEDURE arg:[name: [REF Clearinghouse3::ObjectName], newProperty: [REF Clearinghouse3::Property], value: [REF Clearinghouse3::Item], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName]] error:[ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer]]
        //   [PROCEDURE arg:[name: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], newProperty: LONG_CARDINAL, value: [SEQUENCE (500)UNSPECIFIED]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]] error:[]]
        struct AddItemProperty {
            struct Param {
                ObjectName    name;
                Property      newProperty;
                Item          value;
                Authenticator agent;
            };
            struct Result {
                ObjectName distinguishedObject;
            };

            static const quint16 CODE = 13;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer
        };

        // CONST  RetrieveItem  16  [PROCEDURE arg:[pattern: [REF Clearinghouse3::ObjectNamePattern], property: [REF Clearinghouse3::Property], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName], value: [REF Clearinghouse3::Item]] error:[ArgumentError, AuthenticationError, CallError, PropertyError, WrongServer]]
        //   [PROCEDURE arg:[pattern: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], property: LONG_CARDINAL, agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], value: [SEQUENCE (500)UNSPECIFIED]]] error:[]]
        struct RetrieveItem {
            struct Param {
                ObjectNamePattern pattern;
                Property          property;
                Authenticator     agent;
            };
            struct Result {
                ObjectName distinguishedObject;
                Item       value;
            };

            static const quint16 CODE = 16;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, PropertyError, WrongServer
        };

        // CONST  ChangeItem  17  [PROCEDURE arg:[name: [REF Clearinghouse3::ObjectName], property: [REF Clearinghouse3::Property], newValue: [REF Clearinghouse3::Item], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName]] error:[ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer]]
        //   [PROCEDURE arg:[name: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], property: LONG_CARDINAL, newValue: [SEQUENCE (500)UNSPECIFIED]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]] error:[]]
        struct ChangeItem {
            struct Param {
                ObjectName    name;
                Property      property;
                Item          newValue;
                Authenticator agent;
            };
            struct Result {
                ObjectName distinguishedObject;
            };

            static const quint16 CODE = 17;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer
        };

        // CONST  AddGroupProperty  12  [PROCEDURE arg:[name: [REF Clearinghouse3::ObjectName], newProperty: [REF Clearinghouse3::Property], membership: [REF BulkData1::Source], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName]] error:[ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer]]
        //   [PROCEDURE arg:[name: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], newProperty: LONG_CARDINAL, membership: [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]] error:[]]
        struct AddGroupProperty {
            struct Param {
                ObjectName        name;
                Property          newProperty;
                BulkData1::Source membership;
                Authenticator     agent;
            };
            struct Result {
                ObjectName distinguishedObject;
            };

            static const quint16 CODE = 12;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer
        };

        // CONST  RetrieveMembers  18  [PROCEDURE arg:[pattern: [REF Clearinghouse3::ObjectNamePattern], property: [REF Clearinghouse3::Property], membership: [REF BulkData1::Sink], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName]] error:[ArgumentError, AuthenticationError, CallError, PropertyError, WrongServer]]
        //   [PROCEDURE arg:[pattern: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], property: LONG_CARDINAL, membership: [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]] error:[]]
        struct RetrieveMembers {
            struct Param {
                ObjectNamePattern pattern;
                Property          property;
                BulkData1::Sink   membership;
                Authenticator     agent;
            };
            struct Result {
                ObjectName distinguishedObject;
            };

            static const quint16 CODE = 18;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, PropertyError, WrongServer
        };

        // CONST  AddMember  19  [PROCEDURE arg:[name: [REF Clearinghouse3::ObjectName], property: [REF Clearinghouse3::Property], newMember: [REF Clearinghouse3::ThreePartName], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName]] error:[ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer]]
        //   [PROCEDURE arg:[name: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], property: LONG_CARDINAL, newMember: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]] error:[]]
        struct AddMember {
            struct Param {
                ObjectName    name;
                Property      property;
                ThreePartName newMember;
                Authenticator agent;
            };
            struct Result {
                ObjectName distinguishedObject;
            };

            static const quint16 CODE = 19;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer
        };

        // CONST  AddSelf  20  [PROCEDURE arg:[name: [REF Clearinghouse3::ObjectName], property: [REF Clearinghouse3::Property], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName]] error:[ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer]]
        //   [PROCEDURE arg:[name: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], property: LONG_CARDINAL, agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]] error:[]]
        struct AddSelf {
            struct Param {
                ObjectName    name;
                Property      property;
                Authenticator agent;
            };
            struct Result {
                ObjectName distinguishedObject;
            };

            static const quint16 CODE = 20;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer
        };

        // CONST  DeleteMember  21  [PROCEDURE arg:[name: [REF Clearinghouse3::ObjectName], property: [REF Clearinghouse3::Property], member: [REF Clearinghouse3::ThreePartName], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName]] error:[ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer]]
        //   [PROCEDURE arg:[name: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], property: LONG_CARDINAL, member: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]] error:[]]
        struct DeleteMember {
            struct Param {
                ObjectName    name;
                Property      property;
                ThreePartName member;
                Authenticator agent;
            };
            struct Result {
                ObjectName distinguishedObject;
            };

            static const quint16 CODE = 21;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer
        };

        // CONST  DeleteSelf  22  [PROCEDURE arg:[name: [REF Clearinghouse3::ObjectName], property: [REF Clearinghouse3::Property], agent: [REF Clearinghouse3::Authenticator]] result:[distinguishedObject: [REF Clearinghouse3::ObjectName]] error:[ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer]]
        //   [PROCEDURE arg:[name: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], property: LONG_CARDINAL, agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]] error:[]]
        struct DeleteSelf {
            struct Param {
                ObjectName    name;
                Property      property;
                Authenticator agent;
            };
            struct Result {
                ObjectName distinguishedObject;
            };

            static const quint16 CODE = 22;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, PropertyError, UpdateError, WrongServer
        };

        // CONST  IsMember  23  [PROCEDURE arg:[memberOf: [REF Clearinghouse3::ObjectNamePattern], property: [REF Clearinghouse3::Property], secondaryProperty: [REF Clearinghouse3::Property], name: [REF Clearinghouse3::ThreePartName], agent: [REF Clearinghouse3::Authenticator]] result:[isMember: BOOLEAN, distinguishedObject: [REF Clearinghouse3::ObjectName]] error:[ArgumentError, AuthenticationError, CallError, PropertyError, WrongServer]]
        //   [PROCEDURE arg:[memberOf: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], property: LONG_CARDINAL, secondaryProperty: LONG_CARDINAL, name: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[isMember: BOOLEAN, distinguishedObject: RECORD [organization: [REF Clearinghouse3::Organization], domain: [REF Clearinghouse3::Domain], object: [REF Clearinghouse3::Object]]] error:[]]
        struct IsMember {
            struct Param {
                ObjectNamePattern memberOf;
                Property          property;
                Property          secondaryProperty;
                ThreePartName     name;
                Authenticator     agent;
            };
            struct Result {
                BOOLEAN    isMember;
                ObjectName distinguishedObject;
            };

            static const quint16 CODE = 23;

            using call = void (*)(Param& param, Result& result);
            // throw ArgumentError, AuthenticationError, CallError, PropertyError, WrongServer
        };

        // CONST  RetrieveAddresses  0  [PROCEDURE arg:[] result:[address: [REF Clearinghouse3::NetworkAddressList]] error:[CallError]]
        //   [PROCEDURE arg:[] result:[address: [SEQUENCE (40)[REF Clearinghouse3::NetworkAddress]]]] error:[]]
        struct RetrieveAddresses {
            struct Param {
            };
            struct Result {
                NetworkAddressList address;
            };

            static const quint16 CODE = 0;

            using call = void (*)(Param& param, Result& result);
            // throw CallError
        };

        // CONST  ListDomainServed  1  [PROCEDURE arg:[domains: [REF BulkData1::Sink], agent: [REF Clearinghouse3::Authenticator]] result:[] error:[AuthenticationError, CallError]]
        //   [PROCEDURE arg:[domains: [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]], agent: RECORD [credentials: [REF Authentication3::Credentials], verifier: [REF Authentication3::Verifier]]] result:[] error:[]]
        struct ListDomainServed {
            struct Param {
                BulkData1::Sink domains;
                Authenticator   agent;
            };
            struct Result {
            };

            static const quint16 CODE = 1;

            using call = void (*)(Param& param, Result& result);
            // throw AuthenticationError, CallError
        };
    }

    //
    // Enum Function Declaration
    //
    QString toString(const Courier::Clearinghouse3::WhichArgument                     value);
    QString toString(const Courier::Clearinghouse3::ArgumentProblem                   value);
    QString toString(const Courier::Clearinghouse3::CallProblem                       value);
    QString toString(const Courier::Clearinghouse3::PropertyProblem                   value);
    QString toString(const Courier::Clearinghouse3::UpdateProblem                     value);

    void serialize(BLOCK& block, const Courier::Clearinghouse3::WhichArgument                     value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ArgumentProblem                   value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::CallProblem                       value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::PropertyProblem                   value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::UpdateProblem                     value);

    void deserialize(BLOCK& block, Courier::Clearinghouse3::WhichArgument&                     value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ArgumentProblem&                   value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::CallProblem&                       value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::PropertyProblem&                   value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::UpdateProblem&                     value);

    //
    // Record Function Declaration
    //
    QString toString(const Courier::Clearinghouse3::TwoPartName&                                            value);
    QString toString(const Courier::Clearinghouse3::ThreePartName&                                          value);
    QString toString(const Courier::Clearinghouse3::NetworkAddress&                                         value);
    QString toString(const Courier::Clearinghouse3::Authenticator&                                          value);
    QString toString(const Courier::Clearinghouse3::ArgumentError&                                          value);
    QString toString(const Courier::Clearinghouse3::AuthenticationError&                                    value);
    QString toString(const Courier::Clearinghouse3::CallError&                                              value);
    QString toString(const Courier::Clearinghouse3::PropertyError&                                          value);
    QString toString(const Courier::Clearinghouse3::UpdateError&                                            value);
    QString toString(const Courier::Clearinghouse3::WrongServer&                                            value);
    QString toString(const Courier::Clearinghouse3::CreateObject::Param&                                    value);
    QString toString(const Courier::Clearinghouse3::CreateObject::Result&                                   value);
    QString toString(const Courier::Clearinghouse3::DeleteObject::Param&                                    value);
    QString toString(const Courier::Clearinghouse3::DeleteObject::Result&                                   value);
    QString toString(const Courier::Clearinghouse3::LookupObject::Param&                                    value);
    QString toString(const Courier::Clearinghouse3::LookupObject::Result&                                   value);
    QString toString(const Courier::Clearinghouse3::ListOrganizations::Param&                               value);
    QString toString(const Courier::Clearinghouse3::ListOrganizations::Result&                              value);
    QString toString(const Courier::Clearinghouse3::ListDomain::Param&                                      value);
    QString toString(const Courier::Clearinghouse3::ListDomain::Result&                                     value);
    QString toString(const Courier::Clearinghouse3::ListObjects::Param&                                     value);
    QString toString(const Courier::Clearinghouse3::ListObjects::Result&                                    value);
    QString toString(const Courier::Clearinghouse3::ListAliasesOf::Param&                                   value);
    QString toString(const Courier::Clearinghouse3::ListAliasesOf::Result&                                  value);
    QString toString(const Courier::Clearinghouse3::CreateAlias::Param&                                     value);
    QString toString(const Courier::Clearinghouse3::CreateAlias::Result&                                    value);
    QString toString(const Courier::Clearinghouse3::DeleteAlias::Param&                                     value);
    QString toString(const Courier::Clearinghouse3::DeleteAlias::Result&                                    value);
    QString toString(const Courier::Clearinghouse3::ListAliases::Param&                                     value);
    QString toString(const Courier::Clearinghouse3::ListAliases::Result&                                    value);
    QString toString(const Courier::Clearinghouse3::DeleteProperty::Param&                                  value);
    QString toString(const Courier::Clearinghouse3::DeleteProperty::Result&                                 value);
    QString toString(const Courier::Clearinghouse3::ListProperties::Param&                                  value);
    QString toString(const Courier::Clearinghouse3::ListProperties::Result&                                 value);
    QString toString(const Courier::Clearinghouse3::AddItemProperty::Param&                                 value);
    QString toString(const Courier::Clearinghouse3::AddItemProperty::Result&                                value);
    QString toString(const Courier::Clearinghouse3::RetrieveItem::Param&                                    value);
    QString toString(const Courier::Clearinghouse3::RetrieveItem::Result&                                   value);
    QString toString(const Courier::Clearinghouse3::ChangeItem::Param&                                      value);
    QString toString(const Courier::Clearinghouse3::ChangeItem::Result&                                     value);
    QString toString(const Courier::Clearinghouse3::AddGroupProperty::Param&                                value);
    QString toString(const Courier::Clearinghouse3::AddGroupProperty::Result&                               value);
    QString toString(const Courier::Clearinghouse3::RetrieveMembers::Param&                                 value);
    QString toString(const Courier::Clearinghouse3::RetrieveMembers::Result&                                value);
    QString toString(const Courier::Clearinghouse3::AddMember::Param&                                       value);
    QString toString(const Courier::Clearinghouse3::AddMember::Result&                                      value);
    QString toString(const Courier::Clearinghouse3::AddSelf::Param&                                         value);
    QString toString(const Courier::Clearinghouse3::AddSelf::Result&                                        value);
    QString toString(const Courier::Clearinghouse3::DeleteMember::Param&                                    value);
    QString toString(const Courier::Clearinghouse3::DeleteMember::Result&                                   value);
    QString toString(const Courier::Clearinghouse3::DeleteSelf::Param&                                      value);
    QString toString(const Courier::Clearinghouse3::DeleteSelf::Result&                                     value);
    QString toString(const Courier::Clearinghouse3::IsMember::Param&                                        value);
    QString toString(const Courier::Clearinghouse3::IsMember::Result&                                       value);
    QString toString(const Courier::Clearinghouse3::RetrieveAddresses::Param&                               value);
    QString toString(const Courier::Clearinghouse3::RetrieveAddresses::Result&                              value);
    QString toString(const Courier::Clearinghouse3::ListDomainServed::Param&                                value);
    QString toString(const Courier::Clearinghouse3::ListDomainServed::Result&                               value);

    void serialize(BLOCK& block, const Courier::Clearinghouse3::TwoPartName&                                            value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ThreePartName&                                          value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::NetworkAddress&                                         value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::Authenticator&                                          value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ArgumentError&                                          value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::AuthenticationError&                                    value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::CallError&                                              value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::PropertyError&                                          value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::UpdateError&                                            value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::WrongServer&                                            value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::CreateObject::Param&                                    value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::CreateObject::Result&                                   value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::DeleteObject::Param&                                    value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::DeleteObject::Result&                                   value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::LookupObject::Param&                                    value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::LookupObject::Result&                                   value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ListOrganizations::Param&                               value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ListOrganizations::Result&                              value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ListDomain::Param&                                      value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ListDomain::Result&                                     value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ListObjects::Param&                                     value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ListObjects::Result&                                    value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ListAliasesOf::Param&                                   value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ListAliasesOf::Result&                                  value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::CreateAlias::Param&                                     value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::CreateAlias::Result&                                    value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::DeleteAlias::Param&                                     value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::DeleteAlias::Result&                                    value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ListAliases::Param&                                     value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ListAliases::Result&                                    value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::DeleteProperty::Param&                                  value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::DeleteProperty::Result&                                 value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ListProperties::Param&                                  value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ListProperties::Result&                                 value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::AddItemProperty::Param&                                 value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::AddItemProperty::Result&                                value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::RetrieveItem::Param&                                    value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::RetrieveItem::Result&                                   value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ChangeItem::Param&                                      value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ChangeItem::Result&                                     value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::AddGroupProperty::Param&                                value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::AddGroupProperty::Result&                               value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::RetrieveMembers::Param&                                 value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::RetrieveMembers::Result&                                value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::AddMember::Param&                                       value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::AddMember::Result&                                      value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::AddSelf::Param&                                         value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::AddSelf::Result&                                        value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::DeleteMember::Param&                                    value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::DeleteMember::Result&                                   value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::DeleteSelf::Param&                                      value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::DeleteSelf::Result&                                     value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::IsMember::Param&                                        value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::IsMember::Result&                                       value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::RetrieveAddresses::Param&                               value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::RetrieveAddresses::Result&                              value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ListDomainServed::Param&                                value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::ListDomainServed::Result&                               value);

    void deserialize(BLOCK& block, Courier::Clearinghouse3::TwoPartName&                                            value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ThreePartName&                                          value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::NetworkAddress&                                         value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::Authenticator&                                          value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ArgumentError&                                          value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::AuthenticationError&                                    value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::CallError&                                              value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::PropertyError&                                          value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::UpdateError&                                            value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::WrongServer&                                            value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::CreateObject::Param&                                    value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::CreateObject::Result&                                   value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::DeleteObject::Param&                                    value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::DeleteObject::Result&                                   value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::LookupObject::Param&                                    value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::LookupObject::Result&                                   value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ListOrganizations::Param&                               value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ListOrganizations::Result&                              value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ListDomain::Param&                                      value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ListDomain::Result&                                     value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ListObjects::Param&                                     value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ListObjects::Result&                                    value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ListAliasesOf::Param&                                   value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ListAliasesOf::Result&                                  value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::CreateAlias::Param&                                     value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::CreateAlias::Result&                                    value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::DeleteAlias::Param&                                     value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::DeleteAlias::Result&                                    value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ListAliases::Param&                                     value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ListAliases::Result&                                    value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::DeleteProperty::Param&                                  value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::DeleteProperty::Result&                                 value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ListProperties::Param&                                  value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ListProperties::Result&                                 value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::AddItemProperty::Param&                                 value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::AddItemProperty::Result&                                value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::RetrieveItem::Param&                                    value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::RetrieveItem::Result&                                   value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ChangeItem::Param&                                      value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ChangeItem::Result&                                     value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::AddGroupProperty::Param&                                value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::AddGroupProperty::Result&                               value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::RetrieveMembers::Param&                                 value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::RetrieveMembers::Result&                                value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::AddMember::Param&                                       value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::AddMember::Result&                                      value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::AddSelf::Param&                                         value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::AddSelf::Result&                                        value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::DeleteMember::Param&                                    value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::DeleteMember::Result&                                   value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::DeleteSelf::Param&                                      value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::DeleteSelf::Result&                                     value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::IsMember::Param&                                        value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::IsMember::Result&                                       value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::RetrieveAddresses::Param&                               value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::RetrieveAddresses::Result&                              value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ListDomainServed::Param&                                value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::ListDomainServed::Result&                               value);

    //
    // Choice Function Declaration
    //
    QString toString(const Courier::Clearinghouse3::StreamOfDomain&        value);
    QString toString(const Courier::Clearinghouse3::StreamOfDomainName&    value);
    QString toString(const Courier::Clearinghouse3::StreamOfObject&        value);
    QString toString(const Courier::Clearinghouse3::StreamOfObjectName&    value);
    QString toString(const Courier::Clearinghouse3::StreamOfOrganization&  value);
    QString toString(const Courier::Clearinghouse3::StreamOfThreePartName& value);

    void serialize(BLOCK& block, const Courier::Clearinghouse3::StreamOfDomain&        value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::StreamOfDomainName&    value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::StreamOfObject&        value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::StreamOfObjectName&    value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::StreamOfOrganization&  value);
    void serialize(BLOCK& block, const Courier::Clearinghouse3::StreamOfThreePartName& value);

    void deserialize(BLOCK& block, Courier::Clearinghouse3::StreamOfDomain&        value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::StreamOfDomainName&    value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::StreamOfObject&        value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::StreamOfObjectName&    value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::StreamOfOrganization&  value);
    void deserialize(BLOCK& block, Courier::Clearinghouse3::StreamOfThreePartName& value);
}
#endif
