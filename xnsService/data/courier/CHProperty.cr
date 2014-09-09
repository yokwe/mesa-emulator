CHProperty: PROGRAM 0 VERSION 0 =
BEGIN

-- Propertys for the clearinghouse are broken down into three subsets, primary
-- properties (the right hand side for these is a comment), secondary properties,
-- and generic properties.

-- The first 10,000 Property ids are reserved for generic properties and
-- properties used internally by the clearinghouse and mail services.  The
-- next 10,000 Property ids ([10000..19999]) are reserved for primary properties.
-- The next 10,000 ([20000..29999]) are reserved for secondary properties.
-- The next 10,000 ([30000..39999]) are reserved for associated properties.

Property: TYPE = LONG CARDINAL;

-- Clearinghouse/Mail Internal Stuff (not for clients!):
all: Property = 0;
alias: Property = 1;
aliases: Property = 2;


members: Property = 3;  -- shouldn't be in the generic properties, but
-- members isn't changing for Services 8, so it will just have to make
-- itself comfortable with foreigners.

nsAddress: Property = 4;
authKeys: Property = 6;
authenticationLevel: Property = 8;


-- Definitions from Services 3.0, 4.0, and 5.0 (for backward compability):

ch3fileserver: Property = 10;
ch3printserver: Property = 11;
ch3router: Property = 12;
ch3user: Property = 14;
ch3mailserver: Property = 15;
ch3workstation: Property = 17;
ch3ecs: Property = 20;
ch3its: Property = 23;
ch3gws: Property = 24;
ch3chs: Property = 25;
ch4rs232cPort: Property = 27;
ch4ciu: Property = 28;
ch5ibm3270host: Property = 54;

mailPrimary: Property = 30;
mailboxes: Property = 31;
mailboxAccessCtl: Property = 32;

services: Property = 51;

notUsable: Property = 37777777777B;

-- Property ids [9950..9999] are reserved for experimental purposes

-- primary properties

fs: Property = 10000;  -- file service
ps: Property = 10001;  -- print service
irs: Property = 10002;  -- internetwork routing service
user: Property = 10003;
ms: Property = 10004;  -- mail service
workstation: Property = 10005;
ecs: Property = 10006;  -- external communication service
rs232CPort: Property = 10007;
its: Property = 10008;  -- interactive terminal service
gws: Property = 10009;  -- gateway service
ibm3270Host: Property = 10010;
mailGateway: Property = 10011;
siemens9750Host: Property = 10012;
adobeService: Property = 10013;
librarianService: Property = 10014;
ttxGateway: Property = 10015;  -- teletex gateway Service
as: Property = 10016;  -- authentication service
rbs: Property = 10017;  -- remote batch service
network: Property = 10018;
networkServers: Property = 10019;
ciu: Property = 10020;  -- communications interface unit
chs: Property = 10021;  -- clearinghouse service
userGroup: Property = 10022;
fetchService: Property = 10023;

-- Property ids [19950..19999] are reserved for experimental purposes.

-- secondary properties

userData: Property = 20000;
rs232CData: Property = 20001;
ibm3270HostData: Property = 20002;
siemens9750HostData: Property = 20003;
canMailTo: Property = 20005;   		-- used with user groups
mailGatewayRouteData: Property = 20006;
foreignMailSystemName: Property = 20007;	-- for mail gateway


-- new properties for backward compatibility (When the entry formats
-- changed, some of the information that was available to clients 
-- disappeared.  In order to be backward compatible, we need to be able
-- to supply it to old clients.)

userPassword: Property = 20101;
rs232CBack: Property = 20102;
ibm3270HostBack: Property = 20103;

-- Property IDs [29950..29999] are reserved for experimental purposes.


--associated properties

associatedWorkstation: Property = 30005;

END.
