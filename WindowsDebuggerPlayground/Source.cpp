#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <Windows.h>
#include <intrin.h>
#include <bitset>

//extern void __stdcall Sleep(unsigned long);

enum Registers{
	EAX,
	EBX,
	ECX,
	EDX
};

static bool IsRunningInHyperVCompliantHypervisor()
{
	Registers regs[sizeof(Registers)];

	__cpuid((int*)regs, 1);
	return (regs[ECX] & 0x80000000) != 0;
}

static void IdentifyHypervisor()
{
	std::ios_base::fmtflags f(std::cout.flags());
	Registers regs[sizeof(Registers)];

	std::cout << "Leaf 0x40000000 - Hypervisor CPUID leaf range and vendor ID signature" << std::endl;
	__cpuid((int*)regs, 0x40000000);
	std::cout << "CPUID Leaf Range: " << regs[EAX] << std::endl;
	std::string signature;
	for (const Registers r : { EBX, ECX, EDX })
	{
		
		int iid = regs[r];

		while (iid > 0)
		{
			signature += (char)(iid & 0x000000ff);
			iid >>= 8;
		}
		
	}
	std::cout << "Vendor ID Signature: " << signature << std::endl;
	std::cout << std::endl << std::endl;


	std::cout << "Leaf 0x40000001 - Hypervisor Interface Signature" << std::endl;
	__cpuid((int*)regs, 0x40000001);
	std::string interfaceId;
	for (const Registers r : { EAX })
	{

		int iid = regs[r];

		while (iid > 0)
		{
			interfaceId += (char)(iid & 0x000000ff);
			iid >>= 8;
		}

	}
	std::cout << "Hypervisor Interface Signature: " << interfaceId << std::endl;
	std::cout << std::endl << std::endl;

	
	std::cout << "Leaf 0x40000002 - Hypervisor system identity " << std::endl;
	__cpuid((int*)regs, 0x40000002);
	std::cout << "Build Number: " << regs[EAX] << std::endl;
	std::cout << "Major.Minor: " << ((regs[EBX] & 0xffff0000) >> 16) << '.' << (regs[EBX] & 0x0000ffff) << std::endl;
	std::cout << "Service Pack: " << regs[ECX] << std::endl;
	std::cout << "ServiceBranch.ServiceNumber " << ((regs[EDX] & 0xffffff00) >> 8) << '.' << (regs[EDX] & 0x000000ff) << std::endl;
	std::cout << std::endl << std::endl;


	std::cout << "Leaf 0x40000003 - Hypervisor feature identification " << std::endl;
	__cpuid((int*)regs, 0x40000003);
	std::cout << "Features: " << std::hex << regs[EAX] << std::endl;
	std::cout << "Flags: " << std::hex << regs[EBX] << std::endl;
	std::cout << "Power Management: " << std::hex << regs[ECX] << std::endl;
	std::cout << "Misc " << regs[EDX] << std::endl;
	std::cout << "Partition Privileges " << std::endl;
	std::cout << "------------------------------------" << std::endl;
	std::bitset<32> privileges(regs[EAX]);
	/*
		Bit 0 AccessVpRunTimeMsr				Optional 
		Bit 1 AccessPartitionReferenceCounter	Optional 
		Bit 2 AccessSynicMsrs					Optional 
		Bit 3 AccessSyntheticTimerMsrs			Optional 
		Bit 4 AccessApicMsrs					Optional 
		Bit 5 AccessHypercallMsrs				Must be set 
		Bit 6 AccessVpIndex						Must be set 
		Bit 7 AccessResetMsr					Optional 
		Bit 8 AccessStatsMsr					Optional
		Bit 9 AccessPartitionReferenceTsc		Optional 
		Bit 10 AccessGuestIdleMsr				Optional 
		Bit 11 AccessFrequencyMsrs				Optional 
		Bits 12-31								Reserved
	*/
	if (privileges[0])
	{
		std::cout << "AccessVpnRunTimeMsr" << std::endl;
	}
	if (privileges[1])
	{
		std::cout << "AccessPartitionReferenceCounter" << std::endl;
	}
	if (privileges[2])
	{
		std::cout << "AccessSynicMsrs" << std::endl;
	}
	if (privileges[3])
	{
		std::cout << "AccessSyntheticTimerMsrs" << std::endl;
	}
	if (privileges[4])
	{
		std::cout << "AccessApicMsrs" << std::endl;
	}
	if (privileges[5])
	{
		std::cout << "AccessHypercallMsrs" << std::endl;
	}
	if (privileges[6])
	{
		std::cout << "AccessVpIndex" << std::endl;
	}
	if (privileges[7])
	{
		std::cout << "AccessResetMsr" << std::endl;
	}
	if (privileges[8])
	{
		std::cout << "AccessStatsMsr" << std::endl;
	}
	if (privileges[9])
	{
		std::cout << "AccessPartitionReferenceTsc" << std::endl;
	}
	if (privileges[10])
	{
		std::cout << "AccessGuestIdleMsr" << std::endl;
	}
	if (privileges[11])
	{
		std::cout << "AccessFrequencyMsrs" << std::endl;
	}
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Feature Identification " << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	std::bitset<32> features(regs[EBX]);
	/*
		Bit 0: CreatePartitions			Must be clear 
		Bit 1: AccessPartitionId		Must be clear 
		Bit 2: AccessMemoryPool			Must be clear 
		Bit 3: AdjustMessageBuffers		Must be clear 
		Bit 4: PostMessages				Optional 
		Bit 5: SignalEvents				Optional 
		Bit 6: CreatePort				Must be clear 
		Bit 7: ConnectPort				Optional 
		Bit 8: AccessStats				Must be clear 
		Bit 9-10:						Reserved2  
		Bit 11:	Debugging				Optional 
		Bit 12: CpuManagement			Must be clear 
		Bit 13: ConfigureProfiler		Must be clear 
		Bit 14-31:						Reserved3
	*/
	if (features[0])
	{
		std::cout << "CreatePartitions" << std::endl;
	}
	if (features[1])
	{
		std::cout << "AccessPartitionId" << std::endl;
	}
	if (features[2])
	{
		std::cout << "AccessMemoryPool" << std::endl;
	}
	if (features[3])
	{
		std::cout << "AdjustMessageBuffers" << std::endl;
	}
	if (features[4])
	{
		std::cout << "PostMessages" << std::endl;
	}
	if (features[5])
	{
		std::cout << "SignalEvents" << std::endl;
	}
	if (features[6])
	{
		std::cout << "CreatePort" << std::endl;
	}
	if (features[7])
	{
		std::cout << "ConnectPort" << std::endl;
	}
	if (features[8])
	{
		std::cout << "AccessStats" << std::endl;
	}
	if (features[11])
	{
		std::cout << "Debugging" << std::endl;
	}
	if (features[12])
	{
		std::cout << "CpuManagement" << std::endl;
	}
	if (features[13])
	{
		std::cout << "ConfigureProfiler" << std::endl;
	}
	std::cout << "-----------------------------------" << std::endl;
	std::cout << std::endl << std::endl;

	std::cout << "Leaf 0x40000004 - Enlightenment Implementation Recommendations " << std::endl;
	__cpuid((int*)regs, 0x40000004);
	std::cout << "Implementation Recommendations: " << std::hex << regs[EAX] << std::endl;
	std::cout << "Number of attempts to retry a spinlock: " << std::hex << regs[EBX] << std::endl;
	std::cout << std::endl << std::endl;

	std::cout.flags(f);
	std::cout << "Leaf 0x40000005 - Implementation Limits " << std::endl;
	__cpuid((int*)regs, 0x40000005);
	std::cout << "Virtual Processors Supported: " << regs[EAX] << std::endl;
	std::cout << "Logical Processors Supported: " << regs[EBX] << std::endl;
	std::cout << std::endl << std::endl;
}

int main(int argc, char** argv)
{
	if (IsRunningInHyperVCompliantHypervisor())
	{
		std::cout << "Running in a Hyper-V compliant hypervisor" << std::endl;
		IdentifyHypervisor();
	}
	else
	{
		std::cout << "Not running in a Hyper-V compliant hypervisor" << std::endl;
		IdentifyHypervisor();
	}

	std::cout << "we're just gonna hang out forever until something happens" << std::endl;
	std::string password("password");
	int i = 0;

	while (true)
	{
		Sleep(500);
		std::cout << "still sleeping: " << i++ << std::endl;
	}

	return 0;
}