%-------------------------------------------------------------------------------
  %
  %  Filename       : dbscanMine.m
  %  Author         : Huang Leilei
  %  Created        : 2020-07-27
  %  Description    : my dbscan
  %
%-------------------------------------------------------------------------------

function [idxGrp, idxKnl] = dbscanMine(data, epsilon, minpts, func)

    [idxGrp, idxKnl] = dbscan(data, epsilon, minpts, 'distance', func);

end
