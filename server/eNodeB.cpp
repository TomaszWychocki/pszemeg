#include "eNodeB.h"
#include "PBCH.h"
#include "PDSCH.h"
#include "PDSCH.h"
#include "PRACH.h"
#include "SRB.h"
#include "ports.h"

#include <vector>
#include <iostream>

eNodeB::eNodeB() :
pbch(Ports::pbch_port, 0.5),
pdcch(Ports::pdcch_port, 1),
pdsch(Ports::pdsch_port),
prach(Ports::prach_port, ue_to_handle, clients),
pusch(Ports::pusch_port, ue_to_handle, clients),
pucch(Ports::pucch_port, clients),
srb(Ports::srb_port)
{
}

void eNodeB::start()
{
    while(true)
    {
        this->pbch.run_timer_job();

        this->pdcch.run_timer_job();

        this->prach.handle_connections();

        this->pdsch.handle_queue(this->ue_to_handle);

        this->pusch.handle_connections();

        this->pucch.handle_connections();

        this->srb.handle_connections();
    }
}