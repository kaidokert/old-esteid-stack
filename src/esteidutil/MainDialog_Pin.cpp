/*!
	\file		MainDialog_Pin.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#include "precompiled.h"
#include "MainDialog.h"
#include "AppSetting.h"
#include "PinChangeDialog.h"
#include "PinChangeSecureDialog.h"

#include <smartcard++/esteid/EstEidCard.h>


void MainDialog::OnPinCounter(wxCommandEvent&  event) 
{
	if (!mCardManager) return;
	EstEidCard card(*mCardManager);
	if (!FindCard(card)) return;
	SetStatusText(_("Reading data.."));
	dword auth,sign;
	card.getKeyUsageCounters(auth,sign);
	wxString prompt = 
			_("Authentication key has been used ") +
			wxString::Format(_T("%d"),auth) + _(" times\n");
	prompt+= _("Signature key has been used ") +
			wxString::Format(_T("%d"),sign) + _(" times\n");
	doPopupInfo(prompt);
	SetStatusText(_T(""));
}

void MainDialog::doPinChange(int type ) {
	if (!mCardManager) return;
	EstEidCard card(*mCardManager);
	if (!FindCard(card)) return;
	std::string oldVal,newVal;

	if (!AppSetting().getUseCTAPI()) {
		PinChangeDialog *dlg = new PinChangeDialog(this,getAppName(),type);
		if (wxID_OK != dlg->ShowModal()) return;
		oldVal = dlg->mInput1.ToAscii();
		newVal = dlg->mInput2.ToAscii();
		}
	else {
		PinChangeSecureDialog *dlg = new PinChangeSecureDialog(this,getAppName(),type);
		if (wxID_OK != dlg->ShowModal()) return;
		wxString tmp;
		tmp = wxString(_T("00")) + dlg->mChoiceOld->GetStringSelection();
		oldVal = tmp.Right(2).ToAscii(); //pad to 2 length
		tmp = wxString(_T("00")) + dlg->mChoiceNew->GetStringSelection();
		newVal = tmp.Right(2).ToAscii();
		}

	wxString prompt;
	byte retriesLeft = 0;
	try {
		switch (type) {
			case CHANGEAUTH :
				if (card.changeAuthPin(newVal,oldVal,retriesLeft))
					prompt = _("Authentication PIN has been changed");
				break;
			case CHANGESIGN:
				if (card.changeSignPin(newVal,oldVal,retriesLeft))
					prompt = _("Signature PIN has been changed");
				break;
			case CHANGEPUK:
				if (card.changePUK(newVal,oldVal,retriesLeft))
					prompt = _("PUK code has been changed");
				break;
			case UNBLOCKAUTH:
				if (card.unblockAuthPin(newVal,oldVal,retriesLeft))
					prompt = _("Authentication PIN has been unblocked");
				break;
			case UNBLOCKSIGN:
				if (card.unblockSignPin(newVal,oldVal,retriesLeft))
					prompt = _("Signature PIN has been unblocked");
				break;
			}
		doPopupInfo(prompt);
	} catch(AuthError &err) {
		if (retriesLeft) retriesLeft--;
		wxString prompt;
		if (type == CHANGEAUTH || type == CHANGESIGN)
			prompt = _("Wrong PIN entered");
		else 
			prompt = _("Wrong PUK entered");
		if ((type == UNBLOCKAUTH ||type == UNBLOCKSIGN) && err.m_blocked )
			prompt = _("New PIN must be different from old PIN");
		else 
			prompt+= wxT("\n") +
				wxString::Format(_("You have %d retries left"),retriesLeft);
		doPopupError(prompt);
	} catch (std::runtime_error &ex) {
		doPopupError(_T("Error occured\nTechnical description:") +
			wxString::FromAscii(ex.what()));
	}
	}

void MainDialog::OnChangeAuthPin(wxCommandEvent&  event) 
{
	doPinChange(CHANGEAUTH);
}
void MainDialog::OnChangeSignPin(wxCommandEvent&  event) 
{
	doPinChange(CHANGESIGN);
}
void MainDialog::OnChangePUK(wxCommandEvent&  event) 
{
	doPinChange(CHANGEPUK);
}
void MainDialog::OnUnblockAuthPin(wxCommandEvent&  event) 
{
	doPinChange(UNBLOCKAUTH);
}
void MainDialog::OnUnblockSignPin(wxCommandEvent&  event) 
{
	doPinChange(UNBLOCKSIGN);
}
