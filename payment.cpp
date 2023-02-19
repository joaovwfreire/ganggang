//payment handling logic
ACTION claimp(name user, name dao_name, uint64_t staked_asset);
ACTION addpayment(const name& dao_name, eosio::asset quantity);

/*
    Claimp
    Claims a payment
    Parameters:
        user: name of the user claiming the payment
        dao_name: name of the DAO
        payment_id: id of the payment
        staked_asset: id of the staked asset
    Return value:
        None
    Example:
        claimp("testuser"_n, "testdao"_n, 1, 1)
    Notes:
        This function is called by the user.
        This function checks if the payment exists.
        This function checks if the payment is not already claimed.
        This function checks if the user owns the staked asset.
        This function calculates the payment amount based on the user's share weight.
        This function transfers the payment amount from the DAO's payer to the user.
        This function adds the payment to the receipts table.
*/
void gangdao::claimp(name user, name dao_name, uint64_t payment_id, uint64_t staked_asset){
    // transfer payment from dao_payer to user
    name dao_payer;
    daos_index daos_table(_self, _self.value);
    auto daos_itr = daos_table.find(dao_name.value);
    if (daos_itr != daos_table.end()){
        dao_payer = daos_itr->dao_payer;
    } else {
        check(false, "dao does not exist");
    }
    require_auth(user);

    payments_index payments_table(_self, dao_name.value);
    auto payments_itr = payments_table.find(payment_id);

    // check if payment exists
    check (payments_itr != payments_table.end(), "payment does not exist");

    uint128_t user_payment_id = (uint128_t(user.value) << 64) | payment_id;
    // check if payment is not claimed by looking at the receipts table by user_and_id
    receipts_index receipts_table(_self, dao_name.value);
    auto by_user_and_id = receipts_table.get_index<"byuserandid"_n>();
    auto receipts_itr = by_user_and_id.find(user_payment_id);
    check (receipts_itr == by_user_and_id.end(), "payment already claimed");

    // issue payment to user based on his share weight 
    // get user share weight at the membership table
    membership_table members_table(_self, dao_name.value);
    auto members_itr = members_table.find(staked_asset);
    check (members_itr != members_table.end(), "asset is not a part of the dao");
    check (members_itr->user == user, "user does not own the asset");
    uint16_t share_weight = members_itr->share_weight;

    // get payment quantity
    eosio::asset payment_quantity = payments_itr->quantity;

    // get total shares
    uint64_t total_shares = payments_itr->total_shares;
    // calculate payment amount
    eosio::asset payment_amount = payment_quantity * share_weight / total_shares;

    // get token contract
    name token_contract;
    auto daos_itr = daos_table.find(dao_name.value);
    if (daos_itr != daos_table.end()){
        token_contract = daos_itr->token_contract;
    } else {
        check(false, "dao does not exist");
    }

    // transfer payment
    action(
        permission_level{_self, "active"_n},
        token_contract,
        "transfer"_n,
        std::make_tuple(dao_payer, user, payment_amount, std::string("payment"))
    ).send();

    // add receipt to receipts table
    receipts_table.emplace(_self, [&](auto& row){
        row.id = receipts_table.available_primary_key();
        row.dao_name = dao_name;
        row.user = user;
        row.payment_id = payment_id;
        row.quantity = payments_itr->quantity;
    });

    
}

/*
    Addpayment
    Adds a payment to the payments table
    Parameters:
        dao_name: name of the DAO
        quantity: quantity of the payment
    Return value:
        None
    Example:
        addpayment("testdao"_n, asset(100, symbol("EOS", 4)))
    Notes:
        This function is called by the DAO's payer.
        This function checks if the DAO exists.
        This function adds the payment to the payments table.
*/
void gangdao::addpayment(const name& dao_name, eosio::asset quantity){
    // find dao_payer
    name dao_payer;
    daos_index daos_table(_self, _self.value);
    auto daos_itr = daos_table.find(dao_name.value);
    if (daos_itr != daos_table.end()){
        dao_payer = daos_itr->dao_payer;
    } else {
        check(false, "dao does not exist");
    }
    require_auth(dao_payer);
    payments_index payments_table(_self, dao_name.value);
    payments_table.emplace(_self, [&](auto& row){
        row.id = payments_table.available_primary_key();
        row.quantity = quantity;
    });
}