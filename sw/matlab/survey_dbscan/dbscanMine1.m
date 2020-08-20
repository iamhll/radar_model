%-------------------------------------------------------------------------------
  %
  %  Filename       : dbscanMine1.m
  %  Author         : Huang Leilei
  %  Created        : 2020-07-27
  %  Description    : my dbscan
  %
%-------------------------------------------------------------------------------
  %
  %  Modified       : 2020-08-17 by HLL
  %  Description    : precomputed cost adopted
  %
%-------------------------------------------------------------------------------

function [idxGrp, idxKnl] = dbscanMine1(cost, epsilon, minpts)

    [idxGrp, idxKnl] = dbscan(cost, epsilon, minpts, 'distance', 'precomputed');

end
