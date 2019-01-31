#pragma once

Message* create(int type)
{
	switch (type)
	{
	case MSG_PGSTATS:
		m = new MPGStats(1.f);
		break;
	case MSG_PGSTATSACK:
		m = new MPGStatsAck(1,"aa");
		break;
	case CEPH_MSG_STATFS:
		m = new MStatfs;
		break;
	case CEPH_MSG_STATFS_REPLY:
		m = new MStatfsReply;
		break;
	case MSG_GETPOOLSTATS:
		m = new MGetPoolStats;
		break;
	default:
		break;
	}
}