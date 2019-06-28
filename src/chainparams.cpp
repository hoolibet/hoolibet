// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x0000037f83314be8e19f8aae8caab9a4ce930b070e9d16521fcfc3a2b91bfc27"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1561366800, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x00000b0800b88dde842410f3c860fffeb3da4d68a744f1ff57407fac7780f98f"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1561366801,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x0000093d249fde301e0cb906368fe4301ae2921a8f443eae6348e9b4207b73c0"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1561366802,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x43;
        pchMessageStart[1] = 0xc6;
        pchMessageStart[2] = 0x5b;
        pchMessageStart[3] = 0xa4;
        vAlertPubKey = ParseHex("043a22fa5be283306f434a4363fb3194772472246fd34029b1bfb5f1c1c295c13a6310570c02126f3f8a20d0dc456bcb894e63b62311b63ebd5a3de6313b9ea1ea");
        nDefaultPort = 21340;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // HooliBet starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 525000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 1 * 60; // HooliBet: 1 day
        nTargetSpacing = 1 * 60;  // HooliBet: 1 minute
        nMaturity = 180;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 500000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 600;
        nModifierUpdateBlock = 1;
        nZerocoinStartHeight = 999999999;
        nZerocoinStartTime = 1893456000; // 01/01/2030 @ 12:00am (UTC)
        nBlockEnforceSerialRange = 999999999; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9080000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 999999999; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 999999999; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 999999999; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0 * COIN; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 999999999; //!> The block that zerocoin v2 becomes active - roughly Tuesday, May 8, 2018 4:00:00 AM GMT

        /**
        python ~/GenesisH0new/genesis.py -a quark-hash -z "Bitcoin set a new 2019 high on June 21 hitting 9800 USD" -t 1561366800 -p 04e94125b274aec04f9040c59f4ce8c2e1c2b08457a773f654d27f789c06ddabb4352f9e0682341da8c301d4c6e9558abb31b6f54e25387ee6eba094a44791f6a2 -v 0
04ffff001d010437426974636f696e207365742061206e657720323031392068696768206f6e204a756e652032312068697474696e67203938303020555344
        algorithm: quark-hash
        merkle hash: 5a0130cf2e4926135cadc7287d5e0d44072cab0262866a0cdb432e9b2a762a34
        pszTimestamp: Bitcoin set a new 2019 high on June 21 hitting 9800 USD
        pubkey: 04e94125b274aec04f9040c59f4ce8c2e1c2b08457a773f654d27f789c06ddabb4352f9e0682341da8c301d4c6e9558abb31b6f54e25387ee6eba094a44791f6a2
        time: 1561366800
        bits: 0x1e0ffff0
        Searching for genesis hash..
        genesis hash found!
        nonce: 879223
        genesis hash: 0000037f83314be8e19f8aae8caab9a4ce930b070e9d16521fcfc3a2b91bfc27
         */
        const char* pszTimestamp = "Bitcoin set a new 2019 high on June 21 hitting 9800 USD";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04e94125b274aec04f9040c59f4ce8c2e1c2b08457a773f654d27f789c06ddabb4352f9e0682341da8c301d4c6e9558abb31b6f54e25387ee6eba094a44791f6a2") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1561366800;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 879223;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x0000037f83314be8e19f8aae8caab9a4ce930b070e9d16521fcfc3a2b91bfc27"));
        assert(genesis.hashMerkleRoot == uint256("0x5a0130cf2e4926135cadc7287d5e0d44072cab0262866a0cdb432e9b2a762a34"));

        vSeeds.push_back(CDNSSeedData("hoolibet.club", "dnsseed.hoolibet.club")); // DNS seed
		vSeeds.push_back(CDNSSeedData("seed1.hoolibet.club", "seed1.hoolibet.club")); // Single node address
        vSeeds.push_back(CDNSSeedData("seed2.hoolibet.club", "seed2.hoolibet.club")); // Single node address
        vSeeds.push_back(CDNSSeedData("seed3.hoolibet.club", "seed3.hoolibet.club")); // Single node address
        vSeeds.push_back(CDNSSeedData("seed4.hoolibet.club", "seed4.hoolibet.club")); // Single node address
        vSeeds.push_back(CDNSSeedData("seed5.hoolibet.club", "seed5.hoolibet.club")); // Single node address
        vSeeds.push_back(CDNSSeedData("seed6.hoolibet.club", "seed5.hoolibet.club")); // Single node address
        vSeeds.push_back(CDNSSeedData("seed7.hoolibet.club", "seed5.hoolibet.club")); // Single node address
        vSeeds.push_back(CDNSSeedData("seed8.hoolibet.club", "seed5.hoolibet.club")); // Single node address
        vSeeds.push_back(CDNSSeedData("seed9.hoolibet.club", "seed5.hoolibet.club")); // Single node address
        vSeeds.push_back(CDNSSeedData("explorer.hoolibet.club", "explorer.hoolibet.club")); // Single node address

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 40); // Mainnet hoolibet addresses start with 'H'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 100); // Mainnet hoolibet script addresses start with 'h'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 42);     // Mainnet private keys start with 'H' or 'J'

        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x33)(0x3D)(0x13).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x33)(0x42)(0x5C).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x66).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04c913d9b6979b180dc6634b20a62e1cd28167ecd7d64e46825bf8d3f2cf797125dfedec99135d8e95ac761616f95a6edef925bd738d4afd21a23c8d7fa0e720fd";
        strObfuscationPoolDummyAddress = "HQQVcDhgHHR7Lwqm7b4og3ibLYA9CYUYWa";
        nStartMasternodePayments = 1561638600; // 06/27/2019 @ 12:30pm (UTC)

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zhbet to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x53;
        pchMessageStart[1] = 0xb4;
        pchMessageStart[2] = 0xf2;
        pchMessageStart[3] = 0xd1;
        vAlertPubKey = ParseHex("048805991b48ae88c6fb22f5073df3d0ec2c701b192c6f038e9ec035e8284327380edb6d52f77fc4e337ec9543870f7f2cb4cd911916dcc202211f928f28d01bf6");
        nDefaultPort = 21440;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 1;
        nTargetTimespan = 1 * 60; // HooliBet: 1 day
        nTargetSpacing = 1 * 60;  // HooliBet: 1 minute
        nLastPOWBlock = 500;
        nMaturity = 60;
        nMasternodeCountDrift = 5;
        nModifierUpdateBlock = 1; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 500000000 * COIN;
        nZerocoinStartHeight = 999999999;
        nZerocoinStartTime = 1893456000;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 9902850; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 999999999; //!> The block that zerocoin v2 becomes active

        genesis.nTime = 1561366801;
        genesis.nNonce = 2952421;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000b0800b88dde842410f3c860fffeb3da4d68a744f1ff57407fac7780f98f"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 100); // Testnet hoolibet addresses start with 'h'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 17);  // Testnet hoolibet script addresses start with '7' or '8'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c'
        // Testnet hoolibet BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x5c)(0x60)(0x63)(0xc1).convert_to_container<std::vector<unsigned char> >();
        // Testnet hoolibet BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x5c)(0x60)(0x51)(0x46).convert_to_container<std::vector<unsigned char> >();
        // Testnet hoolibet BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x06).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "0404abb6d57014e9de25090cb137cefa162efd6126350b0c46047255f1bc3b63f285907c50b91cbe6ab1a9b178972c489ee55b150dd2edc61bf5319c2e8c608422";
        strObfuscationPoolDummyAddress = "hQQVcDhgHHR7Lwqm7b4og3ibLYA9CYUYWa";
        nStartMasternodePayments = 1540288801; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa3;
        pchMessageStart[1] = 0xc4;
        pchMessageStart[2] = 0x7d;
        pchMessageStart[3] = 0x23;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // HooliBet: 1 day
        nTargetSpacing = 1 * 60;        // HooliBet: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1561366802;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 30172;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 21540;
        assert(hashGenesisBlock == uint256("0x0000093d249fde301e0cb906368fe4301ae2921a8f443eae6348e9b4207b73c0"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 21640;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
