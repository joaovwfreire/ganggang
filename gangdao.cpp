// import gangdao.hpp
#include "./include/gangdao.hpp"

// import proposals, payment, governance, staking cpp files
#include "proposals.cpp"
#include "payment.cpp"
#include "governance.cpp"
#include "staking.cpp"




// This file has the onboarding logic and integrates all other concerns.


/*
    Registerdao
    Registers a new DAO
    Parameters:
        dao_name: name of the DAO
        token_contract: name of the contract that manages the DAO's token
        ticker: symbol code of the DAO's token
        dao_payer: name of the account that pays for the DAO's resources
    Return value:
        None
    Example:
        registerdao("testdao"_n, "testtoken"_n, "TEST"_n, "testpayer"_n)
    Notes:
        This function is called by the DAO itself.
        This function checks if the DAO is already registered.
        This function adds the DAO to the daotokens table.
*/
void gangdao::registerdao(name dao_name, name token_contract, symbol_code ticker, name dao_payer){
    require_auth(dao_name);
    // check if dao_name is already registered
    daotokens_index daotokens_table(_self, _self.value);
    auto daotokens_itr = daotokens_table.find(dao_name.value);

    check (daotokens_itr == daotokens_table.end(), "dao_name already registered");

    // add dao to daotokens table
    daotokens_table.emplace(_self, [&](auto& row){
        row.dao_name = dao_name;
        row.token_contract = token_contract;
        row.token_symbol = ticker;
        row.dao_payer = dao_payer;
    });
}


/*
    Setweights
    Sets the weights of a DAO's membership
    Parameters:
        template_id: id of the membership atomicasset 
        dao_name: name of the DAO
        vote_weight: weight of the membership in votes
        share_weight: weight of the membership in shares
    Return value:
        None
    Example:
        setweights(1, "testdao"_n, 1, 1)
    Notes:
        This function is called by the DAO itself.
        This function checks if the DAO is already registered.
        If the membership is not registered, it adds it to the membership_config table.
        If the membership is already registered, it modifies it in the membership_config table.
*/
void gangdao::setweights(uint32_t template_id, name dao_name, uint16_t vote_weight, uint16_t share_weight ){
    require_auth(dao_name);

    // check if dao_name is already registered
    daotokens_index daotokens_table(_self, _self.value);
    auto daotokens_itr = daotokens_table.find(dao_name.value);

    check (daotokens_itr != daotokens_table.end(), "dao_name not registered");

    membership_config_table membership_config(_self, _self.value);
    auto membership_config_itr = membership_config.find(template_id);

    if (membership_config_itr == membership_config.end()){
        membership_config.emplace(_self, [&](auto& row){
            row.template_id = template_id;
            row.dao_name = dao_name;
            row.vote_weight = vote_weight;
            row.share_weight = share_weight;
        });
    } else {
        membership_config.modify(membership_config_itr, _self, [&](auto& row){
            row.vote_weight = vote_weight;
            row.share_weight = share_weight;
        });
    }

}


