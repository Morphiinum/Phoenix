// Copyright (c) 2011-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifdef HAVE_CONFIG_H
#include <config/bitcoin-config.h>
#endif

#include <qt/walletmodeltransaction.h>

#include <policy/policy.h>

WalletModelTransaction::WalletModelTransaction(const QList<SendCoinsRecipient> &_recipients) :
    recipients(_recipients),
    fee(0)
{
}

QList<SendCoinsRecipient> WalletModelTransaction::getRecipients() const
{
    return recipients;
}

CTransactionRef& WalletModelTransaction::getWtx()
{
    return wtx;
}

unsigned int WalletModelTransaction::getTransactionSize()
{
    return wtx ? GetVirtualTransactionSize(*wtx) : 0;
}

CAmount WalletModelTransaction::getTransactionFee() const
{
    return fee;
}

void WalletModelTransaction::setTransactionFee(const CAmount& newFee)
{
    fee = newFee;
}

void WalletModelTransaction::reassignAmounts(interfaces::Wallet& wallet, int nChangePosRet)
{
    std::vector<CTxOutput> outputs = wtx->GetOutputs();

    if (nChangePosRet == -1) {
        for (size_t i = 0; i < outputs.size(); i++) {
            if (wallet.isChange(outputs[i])) {
                nChangePosRet = i;
                break;
            }
        }
    }

    int i = 0;
    for (QList<SendCoinsRecipient>::iterator it = recipients.begin(); it != recipients.end(); ++it)
    {
        SendCoinsRecipient& rcp = (*it);
        {
            if (i == nChangePosRet)
                i++;
            rcp.amount = wallet.getValue(outputs[i]);
            i++;
        }
    }
}

CAmount WalletModelTransaction::getTotalTransactionAmount() const
{
    CAmount totalTransactionAmount = 0;
    for (const SendCoinsRecipient &rcp : recipients)
    {
        totalTransactionAmount += rcp.amount;
    }
    return totalTransactionAmount;
}
