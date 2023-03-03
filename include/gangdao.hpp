#pragma once
#include <eosio/eosio.hpp>
#include "atomicassets.hpp"
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/symbol.hpp>
#include <math.h>

using namespace eosio;
using namespace std;
#define ATOMIC name("atomicassets")

    class [[eosio::contract("gangdao")]] gangdao : public contract 
{
    public:
	using contract::contract;
    gangdao(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds) {}

// onboarding logic
ACTION registerdao(name dao_name, name token_contract, symbol_code ticker, name dao_payer);
ACTION setweights(uint32_t template_id, name dao_name, uint16_t vote_weight, uint16_t share_weight );

//voting governance logic
ACTION votecand(name user, uint64_t election_id, vector<eosio::name> candidates, uint64_t staked_asset);

//registering governance logic
ACTION addcandidate(name cand, uint64_t election_id, name dao_name, uint64_t asset_id);
ACTION startelect(name dao_name, uint64_t end, uint8_t elected_n);
ACTION finishelect(name dao_name, uint64_t election_id);

// proposal governance logic
ACTION propose(name cust, name dao_name, uint64_t asset_id, std::string proposal);
ACTION voteprop(name user, uint64_t proposal_id, uint64_t asset_id, std::string vote);

//payment handling logic
ACTION claimp(name user, name dao_name, uint64_t payment_id, uint64_t staked_asset);
ACTION addpayment(const name& dao_name, eosio::asset quantity);


// staking logic
[[eosio::on_notify("atomicassets::transfer")]]
        void stake(
        const name& from,
        const name& to,
        vector<uint64_t> asset_ids,
        const string memo);

ACTION unstake(name user, uint64_t asset_id);


struct [[eosio::table("custodians"), eosio::contract("gangdao")]] custodian {
        eosio::name cust_name;
        eosio::name dao_name;
        uint64_t total_votes;
        uint64_t cust_end;

        uint64_t primary_key() const { return cust_name.value; }
        uint64_t by_dao_name() const { return dao_name.value; }
        
    };

    typedef eosio::multi_index<"custodians"_n, custodian,
            eosio::indexed_by<"bycustodian"_n, eosio::const_mem_fun<custodian, uint64_t, &custodian::primary_key> >,
            eosio::indexed_by<"bydao"_n, eosio::const_mem_fun<custodian, uint64_t, &custodian::by_dao_name> >
    > custodians_table;


    struct [[eosio::table("candidates"), eosio::contract("gangdao")]] candidate {
        uint64_t cand_id;
        eosio::name cand_name;
        eosio::name  dao_name;
        uint64_t votes;
        uint64_t election_id;

        uint64_t primary_key() const { return cand_id; }
        uint64_t by_cand_name() const { return cand_name.value; }
        uint64_t by_dao_name() const { return dao_name.value; }
        uint64_t by_number_votes() const { return static_cast<uint64_t>(votes); }
        uint128_t by_cand_and_election() const { return (static_cast<uint128_t>(election_id) << 64) | cand_name.value; }
        
    };

    typedef eosio::multi_index<"candidates"_n, candidate,
            eosio::indexed_by<"bycand"_n, eosio::const_mem_fun<candidate, uint64_t, &candidate::primary_key> >,
            eosio::indexed_by<"bycandidate"_n, eosio::const_mem_fun<candidate, uint64_t, &candidate::by_cand_name> >,
            eosio::indexed_by<"byvotes"_n, eosio::const_mem_fun<candidate, uint64_t, &candidate::by_number_votes> >,
            eosio::indexed_by<"bycandelect"_n, eosio::const_mem_fun<candidate, uint128_t, &candidate::by_cand_and_election> >
    > candidates_table;


    struct [[eosio::table]] membership {
        
        uint64_t asset_id;
        eosio::name user;
        eosio::name dao_name;
        uint32_t shares;
        uint32_t votes;
        uint64_t reg_time;
        uint64_t l_p_claim;
        uint64_t l_e_vote;
        

        uint64_t primary_key()const { return asset_id; }
        uint64_t by_user_name() const { return user.value; }
        uint64_t by_dao_name() const { return dao_name.value; }

    };
    typedef eosio::multi_index< "membership"_n, membership,
            eosio::indexed_by<"byasset"_n, eosio::const_mem_fun<membership, uint64_t, &membership::primary_key> >,
            eosio::indexed_by<"byusername"_n, eosio::const_mem_fun<membership, uint64_t, &membership::by_user_name> >,
            eosio::indexed_by<"bydao"_n, eosio::const_mem_fun<membership, uint64_t, &membership::by_dao_name> >
    > membership_table;

    struct [[eosio::table]] memconfigs {
        uint32_t template_id;
        eosio::name dao_name;
        uint32_t vote_weight;
        uint32_t share_weight;

        uint64_t primary_key()const { return template_id; }
        uint64_t by_dao_name() const { return dao_name.value; }
         
        
    };
    typedef eosio::multi_index< "memconfigs"_n, memconfigs, 
            eosio::indexed_by<"bytemplate"_n, eosio::const_mem_fun<memconfigs, uint64_t, &memconfigs::primary_key> >,
            eosio::indexed_by<"bydao"_n, eosio::const_mem_fun<memconfigs, uint64_t, &memconfigs::by_dao_name> >
    > membership_config_table;

    struct [[eosio::table]] totalweights {

        eosio::name dao_name;
        uint32_t vote_totals;
        uint32_t share_totals;

        uint64_t primary_key()const { return dao_name.value; }
    };
    typedef eosio::multi_index< "totalweights"_n, totalweights > totalweights_index;

    struct [[eosio::table]] proposals {
        uint64_t proposal_id;
        eosio::name dao_name;
        eosio::name custodian;
        std::string proposal;
        uint64_t votes_for;
        uint64_t votes_against;
        uint8_t approved;
        uint64_t expiry;



        uint64_t primary_key()const { return proposal_id; }
        uint64_t by_dao_name() const { return dao_name.value; } 
        uint64_t by_custodian() const { return custodian.value; }

    };
    typedef eosio::multi_index< "proposals"_n, proposals,
            eosio::indexed_by<"byproposal"_n, eosio::const_mem_fun<proposals, uint64_t, &proposals::primary_key> >,
            eosio::indexed_by<"bydao"_n, eosio::const_mem_fun<proposals, uint64_t, &proposals::by_dao_name> >,
            eosio::indexed_by<"bycustodian"_n, eosio::const_mem_fun<proposals, uint64_t, &proposals::by_custodian> >
    > proposals_table;

    struct [[eosio::table]] propvotes {
        uint64_t id;
        uint64_t proposal_id;
        eosio::name user;
        uint8_t vote;

        uint64_t primary_key()const { return id; }
        uint64_t by_proposal_id() const { return proposal_id; }
        uint64_t by_user() const { return user.value; }
        uint128_t by_proposal_and_user() const { return (static_cast<uint128_t>(proposal_id) << 64) | user.value; }

    };
    typedef eosio::multi_index< "propvotes"_n, propvotes,
            eosio::indexed_by<"byid"_n, eosio::const_mem_fun<propvotes, uint64_t, &propvotes::primary_key> >,
            eosio::indexed_by<"byproposal"_n, eosio::const_mem_fun<propvotes, uint64_t, &propvotes::by_proposal_id> >,
            eosio::indexed_by<"byuser"_n, eosio::const_mem_fun<propvotes, uint64_t, &propvotes::by_user> >,
            eosio::indexed_by<"bypropuser"_n, eosio::const_mem_fun<propvotes, uint128_t, &propvotes::by_proposal_and_user> >
    > proposalvotes;


    struct [[eosio::table]] elections {
        uint64_t election_id;
        eosio::name dao_name;
        uint64_t start;
        uint64_t end;
        uint8_t elected_n;
        uint64_t votes_issued;
        uint64_t total_votes;

        uint64_t primary_key()const { return election_id; }
        uint64_t by_dao_name() const { return dao_name.value; }

    };
    typedef eosio::multi_index< "elections"_n, elections,
            eosio::indexed_by<"byelection"_n, eosio::const_mem_fun<elections, uint64_t, &elections::primary_key> >,
            eosio::indexed_by<"bydao"_n, eosio::const_mem_fun<elections, uint64_t, &elections::by_dao_name> >
    > elections_table;

    struct [[eosio::table]] daos_p {
        uint64_t payment_id; 
        eosio::name dao_name;
        uint64_t total_shares;
        eosio::asset quantity;
        

        uint64_t primary_key()const { return payment_id; }
        uint64_t by_dao_name() const { return dao_name.value; }
    };
    typedef eosio::multi_index< "daos"_n, daos_p,
            eosio::indexed_by<"byid"_n, eosio::const_mem_fun<daos_p, uint64_t, &daos_p::primary_key> >,
            eosio::indexed_by<"bydaoname"_n, eosio::const_mem_fun<daos_p, uint64_t, &daos_p::by_dao_name> >
    > daos_index;

    struct [[eosio::table]] daotokens{
        eosio::name dao_name;
        eosio::name token_contract;
        eosio::symbol_code token_symbol;
        eosio::name dao_payer;

        uint64_t primary_key() const { return dao_name.value; }
    };

    typedef eosio::multi_index<"daotokens"_n, daotokens> daotokens_index;

    struct [[eosio::table]] receipts {
        uint64_t receipt_id; // -1
        uint64_t payment_id; 
        eosio::name dao_name;
        eosio::name user;
        eosio::asset quantity;
        

        uint64_t primary_key()const { return receipt_id; }
        uint64_t by_payment_id() const { return payment_id; }
        uint64_t by_dao_name() const { return dao_name.value; }
        uint64_t by_user() const { return user.value; }
        uint128_t by_user_and_id() const { return (uint128_t{user.value} << 64) | payment_id; }
    };
    typedef eosio::multi_index< "receipts"_n, receipts,
            eosio::indexed_by<"byid"_n, eosio::const_mem_fun<receipts, uint64_t, &receipts::primary_key> >,
            eosio::indexed_by<"bydaoname"_n, eosio::const_mem_fun<receipts, uint64_t, &receipts::by_dao_name> >,
            eosio::indexed_by<"byuser"_n, eosio::const_mem_fun<receipts, uint64_t, &receipts::by_user> >,
            eosio::indexed_by<"byuserandid"_n, eosio::const_mem_fun<receipts, uint128_t, &receipts::by_user_and_id> >
    > receipts_index;


    HYDRA_FIXTURE_ACTION(
        ((custodians)(custodian)(custodians_table))
        ((candidates)(candidate)(candidates_table))
        ((membership)(membership)(membership_table))
        ((memconfigs)(memconfigs)(membership_config_table))
        ((totalweights)(totalweights)(totalweights_index))
        ((proposals)(proposals)(proposals_table))
        ((propvotes)(propvotes)(proposalvotes))
        ((elections)(elections)(elections_table))
        ((daos)(daos_p)(daos_index))
        ((receipts)(receipts)(receipts_index))
        ((daotokens)(daotokens)(daotokens_index))
    )


struct send_parguments {
      eosio::name user;
      eosio::asset quantity;
    };

struct setweightsarguments {
      uint32_t template_id;
      uint32_t vote_weight;
      uint32_t share_weight;
    };

struct cpsarguments {
      uint32_t template_id;
      uint32_t vote_weight;
      uint32_t share_weight;
    };

struct transferargs {
      name from;
      name to;
      asset quantity;
      string memo;
    };

struct candidate_votes {
      name candidate;
      uint64_t votes;
    };



struct atomictransfer{
        name from;
        name to;
        vector <uint64_t> asset_ids;
        string memo;
  };

struct atomicmintasset{
    name authorized_minter;
    name collection_name;
    name schema_name;
    int32_t template_id;
    name new_asset_owner;
    ATTRIBUTE_MAP immutable_data;
    ATTRIBUTE_MAP mutable_data;
    vector <asset> tokens_to_back;
};

struct nonvectoratomictransfer{
        name from;
        name to;
        vector <uint64_t>  asset_id;
        string memo;
  };  

struct electedcandidate {
      name candidate;
      uint64_t votes;
    };

  };


