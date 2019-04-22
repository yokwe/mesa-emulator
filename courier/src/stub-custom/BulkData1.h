#ifndef STUB_BulkData1_H__
#define STUB_BulkData1_H__

#include "../courier/Courier.h"
#include "../courier/StreamOf.h"

namespace Courier {
	namespace Stub {
    namespace BulkData1 {

        const quint32 PROGRAM_NUMBER = 0;
        const quint32 VERSION_NUMBER = 1;

        // TYPE  StreamOfUnspecified  [CHOICE [[[[nextSegment  0]] => RECORD [segment: [SEQUENCE (65535)UNSPECIFIED]], restOfStream: [REF BulkData1::StreamOfUnspecified]]], [[[lastSegment  1]] => [SEQUENCE (65535)UNSPECIFIED]]]]]
        using StreamOfUnspecified = StreamOf<UNSPECIFIED>;

        // TYPE  Identifier  RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]]
        struct Identifier {
            ARRAY_N<UNSPECIFIED,3> host;
            ARRAY_N<UNSPECIFIED,2> hostRelativeIdentifier;
        };

        // TYPE  Descriptor  [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
        struct Descriptor {
            enum class CHOICE_TAG : quint16 {
                null      = 0,
                immediate = 1,
                passive   = 2,
                active    = 3,
            };
            struct Descriptor_CHOICE_01 {
            };
            struct Descriptor_CHOICE_02 {
                ARRAY_N<UNSPECIFIED,2> network;
                ARRAY_N<UNSPECIFIED,3> host;
                Identifier             identifier;
            };

            CHOICE_TAG choiceTag;

            Descriptor_CHOICE_01& null()      const;
            Descriptor_CHOICE_01& immediate() const;
            Descriptor_CHOICE_02& passive()   const;
            Descriptor_CHOICE_02& active()    const;
        private:
            mutable Descriptor_CHOICE_01  choice_01;
            mutable Descriptor_CHOICE_02  choice_02;
        };

        // TYPE  Sink  [REF BulkData1::Descriptor]
        //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
        using Sink = Descriptor;

        // CONST  immediateSink  immediate [ARRAY (0)[]  [REF BulkData1::Sink]
        //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
        // FIXME CONST CHOICE

        // CONST  nullSink  null [ARRAY (0)[]  [REF BulkData1::Sink]
        //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
        // FIXME CONST CHOICE

        // TYPE  Source  [REF BulkData1::Descriptor]
        //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
        using Source = Descriptor;

        // CONST  immediateSource  immediate [ARRAY (0)[]  [REF BulkData1::Source]
        //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
        // FIXME CONST CHOICE

        // CONST  nullSource  null [ARRAY (0)[]  [REF BulkData1::Source]
        //   [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]]
        // FIXME CONST CHOICE

        // CONST  InvalidDescriptor  0  [ERROR ]
        struct InvalidDescriptor {
            static const quint16 CODE = 0;
        };

        // CONST  NoSuchIdentifier  1  [ERROR ]
        struct NoSuchIdentifier {
            static const quint16 CODE = 1;
        };

        // CONST  IdentifierBusy  2  [ERROR ]
        struct IdentifierBusy {
            static const quint16 CODE = 2;
        };

        // CONST  WrongHost  3  [ERROR ]
        struct WrongHost {
            static const quint16 CODE = 3;
        };

        // CONST  WrongDirection  4  [ERROR ]
        struct WrongDirection {
            static const quint16 CODE = 4;
        };

        // CONST  TransferAborted  5  [ERROR ]
        struct TransferAborted {
            static const quint16 CODE = 5;
        };

        // CONST  Send  0  [PROCEDURE arg:[identifier: [REF BulkData1::Identifier], sink: [REF BulkData1::Sink], timeout: CARDINAL] result:[] error:[InvalidDescriptor, NoSuchIdentifier, IdentifierBusy, WrongHost, WrongDirection, TransferAborted]]
        //   [PROCEDURE arg:[identifier: RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]], sink: [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]], timeout: CARDINAL] result:[] error:[]]
        struct Send {
            struct Param {
                Identifier identifier;
                Sink       sink;
                CARDINAL   timeout;
            };
            struct Result {
            };

            static const quint16 CODE = 0;

            using call = void (*)(Param& param, Result& result);
            // throw InvalidDescriptor, NoSuchIdentifier, IdentifierBusy, WrongHost, WrongDirection, TransferAborted
        };

        // CONST  Receive  1  [PROCEDURE arg:[identifier: [REF BulkData1::Identifier], source: [REF BulkData1::Source], timeout: CARDINAL] result:[] error:[InvalidDescriptor, NoSuchIdentifier, IdentifierBusy, WrongHost, WrongDirection, TransferAborted]]
        //   [PROCEDURE arg:[identifier: RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]], source: [CHOICE [[[[null  0], [immediate  1]] => RECORD []], [[[passive  2], [active  3]] => RECORD [network: [ARRAY (2)UNSPECIFIED]], host: [ARRAY (3)UNSPECIFIED]], identifier: [REF BulkData1::Identifier]]]]], timeout: CARDINAL] result:[] error:[]]
        struct Receive {
            struct Param {
                Identifier identifier;
                Source     source;
                CARDINAL   timeout;
            };
            struct Result {
            };

            static const quint16 CODE = 1;

            using call = void (*)(Param& param, Result& result);
            // throw InvalidDescriptor, NoSuchIdentifier, IdentifierBusy, WrongHost, WrongDirection, TransferAborted
        };

        // CONST  Cancel  2  [PROCEDURE arg:[identifier: [REF BulkData1::Identifier], timeout: CARDINAL] result:[] error:[NoSuchIdentifier, IdentifierBusy, WrongHost]]
        //   [PROCEDURE arg:[identifier: RECORD [host: [ARRAY (3)UNSPECIFIED]], hostRelativeIdentifier: [ARRAY (2)UNSPECIFIED]]], timeout: CARDINAL] result:[] error:[]]
        struct Cancel {
            struct Param {
                Identifier identifier;
                CARDINAL   timeout;
            };
            struct Result {
            };

            static const quint16 CODE = 2;

            using call = void (*)(Param& param, Result& result);
            // throw NoSuchIdentifier, IdentifierBusy, WrongHost
        };
    }
	}

    //
    // Enum Function Declaration
    //
    QString toString(const Courier::Stub::BulkData1::Descriptor::CHOICE_TAG          value);

    void serialize(BLOCK& block, const Courier::Stub::BulkData1::Descriptor::CHOICE_TAG          value);

    void deserialize(BLOCK& block, Courier::Stub::BulkData1::Descriptor::CHOICE_TAG&          value);

    //
    // Record Function Declaration
    //
    QString toString(const Courier::Stub::BulkData1::Identifier&                                         value);
    QString toString(const Courier::Stub::BulkData1::Descriptor::Descriptor_CHOICE_01&                   value);
    QString toString(const Courier::Stub::BulkData1::Descriptor::Descriptor_CHOICE_02&                   value);
    QString toString(const Courier::Stub::BulkData1::InvalidDescriptor&                                  value);
    QString toString(const Courier::Stub::BulkData1::NoSuchIdentifier&                                   value);
    QString toString(const Courier::Stub::BulkData1::IdentifierBusy&                                     value);
    QString toString(const Courier::Stub::BulkData1::WrongHost&                                          value);
    QString toString(const Courier::Stub::BulkData1::WrongDirection&                                     value);
    QString toString(const Courier::Stub::BulkData1::TransferAborted&                                    value);
    QString toString(const Courier::Stub::BulkData1::Send::Param&                                        value);
    QString toString(const Courier::Stub::BulkData1::Send::Result&                                       value);
    QString toString(const Courier::Stub::BulkData1::Receive::Param&                                     value);
    QString toString(const Courier::Stub::BulkData1::Receive::Result&                                    value);
    QString toString(const Courier::Stub::BulkData1::Cancel::Param&                                      value);
    QString toString(const Courier::Stub::BulkData1::Cancel::Result&                                     value);

    void serialize(BLOCK& block, const Courier::Stub::BulkData1::Identifier&                                         value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::Descriptor::Descriptor_CHOICE_01&                   value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::Descriptor::Descriptor_CHOICE_02&                   value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::InvalidDescriptor&                                  value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::NoSuchIdentifier&                                   value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::IdentifierBusy&                                     value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::WrongHost&                                          value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::WrongDirection&                                     value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::TransferAborted&                                    value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::Send::Param&                                        value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::Send::Result&                                       value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::Receive::Param&                                     value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::Receive::Result&                                    value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::Cancel::Param&                                      value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::Cancel::Result&                                     value);

    void deserialize(BLOCK& block, Courier::Stub::BulkData1::Identifier&                                         value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::Descriptor::Descriptor_CHOICE_01&                   value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::Descriptor::Descriptor_CHOICE_02&                   value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::InvalidDescriptor&                                  value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::NoSuchIdentifier&                                   value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::IdentifierBusy&                                     value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::WrongHost&                                          value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::WrongDirection&                                     value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::TransferAborted&                                    value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::Send::Param&                                        value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::Send::Result&                                       value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::Receive::Param&                                     value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::Receive::Result&                                    value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::Cancel::Param&                                      value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::Cancel::Result&                                     value);

    //
    // Choice Function Declaration
    //
//    QString toString(const Courier::Stub::BulkData1::StreamOfUnspecified& value);
    QString toString(const Courier::Stub::BulkData1::Descriptor&          value);

//    void serialize(BLOCK& block, const Courier::Stub::BulkData1::StreamOfUnspecified& value);
    void serialize(BLOCK& block, const Courier::Stub::BulkData1::Descriptor&          value);

//    void deserialize(BLOCK& block, Courier::Stub::BulkData1::StreamOfUnspecified& value);
    void deserialize(BLOCK& block, Courier::Stub::BulkData1::Descriptor&          value);
}
#endif
