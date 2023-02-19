ACTION propose(name cust, name dao_name, uint64_t asset_id, std::string proposal);
ACTION voteprop(name user, uint64_t proposal_id, uint64_t asset_id, std::string vote);

/*
    Propose
    Proposes a new proposal
    Parameters:
        cust: name of the custodian
        dao_name: name of the dao
        asset_id: id of the staked asset
        proposal: proposal
    Return value:
        none
    Notes:
        - proposal_id is the primary key of the proposals table
        - asset_id is the primary key of the membership table
*/
void gangdao::propose(name cust, name dao_name, uint64_t asset_id, std::string proposal){
    require_auth(cust);
    //check if dao exists
    dao_index dao_table(_self, _self.value);
    auto dao_itr = dao_table.find(dao_name.value);
    check (dao_itr != dao_table.end(), "dao does not exist");
    //check if cust is really a custodian

    custodians_index cust_table(_self, dao_name.value);
    auto cust_itr = cust_table.find(cust.value);
    check (cust_itr != cust_table.end(), "custodian does not exist");
    check (cust_itr-> dao_name == dao_name, "custodian does not belong to this dao");
    
    //check if asset_id is a valid asset
    membership_table mem_table(_self, dao_name.value);
    auto mem_itr = mem_table.find(asset_id);
    check (mem_itr != mem_table.end(), "asset not found");
    
    uint64_t proposal_id;
        eosio::name dao_name;
        eosio::name custodian;
        std::string proposal;
        uint64_t votes_for;
        uint64_t votes_against;
        uint8_t approved;
        uint64_t expiry;

    //get current time
    eosio::time_point_sec tps = eosio::current_time_point();
    uint64_t COUNT_SECONDS (tps.sec_since_epoch());

    //create proposal
    proposals_index proposals_table(_self, _self.value);
    proposals_table.emplace(_self, [&](auto& row){
        row.proposal_id = proposals_table.available_primary_key();
        row.proposal = proposal;
        row.custodian = cust;
        row.dao_name = dao_name;
        row.status = "open";
        row.expiry = COUNT_SECONDS + 604800; // 7 days
        row.votes_for = 0;
        row.votes_against = 0;
        row.approved = 0;
    
    });
    
}

/*
    VoteProposal
    Votes on a proposal
    Parameters:
        user: name of the user
        proposal_id: id of the proposal
        asset_id: id of the staked asset
        vote: vote (yes, no, abstain)
    Return value:
        none
    Notes: 
        - proposal_id is the primary key of the proposals table
        - asset_id is the primary key of the membership table
*/

void gangdao::voteprop(name user, uint64_t proposal_id, uint64_t asset_id, std::string vote){
    require_auth(user);
    //check if proposal exists
    proposals_index proposals_table(_self, _self.value);
    auto proposals_itr = proposals_table.find(proposal_id);
    check (proposals_itr != proposals_table.end(), "proposal does not exist");

    //check if user is a member of the dao
    membership_table mem_table(_self, _self.value);
    auto mem_itr = mem_table.find(asset_id);
    check (mem_itr != mem_table.end(), "asset not found");
    check (mem_itr->user == user, "user is not a member of this dao");
    check (mem_itr-> dao_name == proposals_itr->dao_name, "user does not belong to this dao");
    
    //check if vote is valid
    check (vote == "yes" || vote == "no" || vote == "abstain", "invalid vote");

    // check if user has already voted by querying byproposaluser secondary index
    uint128_t proposal_id_user = (uint128_t(proposal_id) << 64) | user.value;
    proposalvotes proptable(_self, _self.value);
    auto by_proposal_user = proptable.get_index<"byproposaluser"_n>();
    auto prop_itr = by_proposal_user.find(proposal_id);
    check (prop_itr == by_proposal_user.end(), "user has already voted on this proposal");

    //check if proposal vote is still open
    eosio::time_point_sec tps = eosio::current_time_point();
    uint64_t COUNT_SECONDS (tps.sec_since_epoch());
    check (COUNT_SECONDS < proposals_itr->expiry, "proposal vote is closed");

    //create proposal vote
    proptable.emplace(_self, [&](auto& row){
        row.id = proptable.available_primary_key();
        row.proposal_id = proposal_id;
        row.user = user;
        row.vote = vote;
    });

    //update proposal vote count
    proposals_table.modify(proposals_itr, _self, [&](auto& row){
        if (vote == "yes"){
            row.votes_for += mem_itr->vote_weight;
        }
        else if (vote == "no"){
            row.votes_against += mem_itr->vote_weight;
        } 
    });

}